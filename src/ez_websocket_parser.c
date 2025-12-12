#include "ez_websocket_parser.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>

/* WebSocket 解析器状态 */
enum ws_state {
	s_ws_frame_start = 0,
	s_ws_frame_opcode,
	s_ws_frame_payload_len,
	s_ws_frame_payload_len_16_1,
	s_ws_frame_payload_len_16_2,
	s_ws_frame_payload_len_64_1,
	s_ws_frame_payload_len_64_2,
	s_ws_frame_payload_len_64_3,
	s_ws_frame_payload_len_64_4,
	s_ws_frame_payload_len_64_5,
	s_ws_frame_payload_len_64_6,
	s_ws_frame_payload_len_64_7,
	s_ws_frame_payload_len_64_8,
	s_ws_frame_mask_key_1,
	s_ws_frame_mask_key_2,
	s_ws_frame_mask_key_3,
	s_ws_frame_mask_key_4,
	s_ws_frame_payload,
	s_ws_frame_complete
};

#define SET_ERRNO(e) \
do { \
	parser->ws_errno = (e); \
} while(0)

#define CURRENT_STATE() ((enum ws_state)parser->state)
#define UPDATE_STATE(V) parser->state = (unsigned int)(V)

#define CALLBACK_NOTIFY_(FOR, ER) \
do { \
	if (settings->on_##FOR) { \
		if (settings->on_##FOR(parser) != 0) { \
			SET_ERRNO(WSE_UNKNOWN); \
			return (ER); \
		} \
	} \
} while (0)

#define CALLBACK_NOTIFY(FOR) CALLBACK_NOTIFY_(FOR, p - data)

/* CALLBACK_DATA_ 宏未使用，直接使用内联代码 */

/* 错误信息表 */
static const struct {
	const char *name;
	const char *description;
} ws_strerror_tab[] = {
	{ "WSE_OK", "success" },
	{ "WSE_INVALID_FRAME", "invalid frame format" },
	{ "WSE_INVALID_OPCODE", "invalid opcode" },
	{ "WSE_INVALID_PAYLOAD_LEN", "invalid payload length" },
	{ "WSE_INVALID_MASK", "invalid mask" },
	{ "WSE_INCOMPLETE_FRAME", "incomplete frame" },
	{ "WSE_PAYLOAD_TOO_LARGE", "payload too large" },
	{ "WSE_UNKNOWN", "unknown error" }
};

/* 应用 mask */
static void apply_mask(uint8_t *data, size_t len, const uint8_t *mask_key, size_t mask_index) {
	for (size_t i = 0; i < len; i++) {
		data[i] ^= mask_key[(mask_index + i) % 4];
	}
}

/* 初始化解析器 */
void websocket_parser_init(websocket_parser *parser) {
	void *data = parser->data; /* 保留用户数据 */
	memset(parser, 0, sizeof(*parser));
	parser->data = data;
	parser->state = s_ws_frame_start;
	parser->ws_errno = WSE_OK;
	parser->close_received = 0;
}

/* 初始化设置 */
void websocket_parser_settings_init(websocket_parser_settings *settings) {
	memset(settings, 0, sizeof(*settings));
}

/* 执行解析 */
size_t websocket_parser_execute(websocket_parser *parser,
                                const websocket_parser_settings *settings,
                                const char *data,
                                size_t len) {
	const char *p = data;
	const char *end = data + len;
	
	/* 如果已经在错误状态，不继续解析 */
	if (WEBSOCKET_PARSER_ERRNO(parser) != WSE_OK) {
		return 0;
	}
	
	if (len == 0) {
		return 0;
	}
	
	while (p < end) {
		unsigned char ch = (unsigned char)*p;
		
		switch (CURRENT_STATE()) {
		case s_ws_frame_start:
			/* 第一个字节：FIN + RSV + Opcode */
			parser->fin = (ch & 0x80) ? 1 : 0;
			parser->opcode = ch & 0x0F;
			
			/* 验证 opcode */
			if (parser->opcode > 0x0A || 
			    (parser->opcode > 0x02 && parser->opcode < 0x08) ||
			    (parser->opcode > 0x0A)) {
				SET_ERRNO(WSE_INVALID_OPCODE);
				return p - data;
			}
			
			CALLBACK_NOTIFY_(frame_begin, p - data);
			if (WEBSOCKET_PARSER_ERRNO(parser) != WSE_OK) {
				return p - data;
			}
			
			UPDATE_STATE(s_ws_frame_opcode);
			p++;
			break;
			
		case s_ws_frame_opcode:
			/* 第二个字节：MASK + Payload length */
			parser->mask = (ch & 0x80) ? 1 : 0;
			parser->has_mask = parser->mask;
			parser->payload_len = ch & 0x7F;
			
		if (parser->payload_len < 126) {
			parser->payload_len_type = 0;
			parser->payload_received = 0;
			if (parser->mask) {
				UPDATE_STATE(s_ws_frame_mask_key_1);
			} else {
				/* 没有 mask，清空 mask 相关字段 */
				parser->mask_index = 0;
				memset(parser->masking_key, 0, sizeof(parser->masking_key));
				if (parser->payload_len == 0) {
					UPDATE_STATE(s_ws_frame_complete);
				} else {
					UPDATE_STATE(s_ws_frame_payload);
				}
			}
			} else if (parser->payload_len == 126) {
				parser->payload_len_type = 1;
				parser->payload_len = 0;
				UPDATE_STATE(s_ws_frame_payload_len_16_1);
			} else if (parser->payload_len == 127) {
				parser->payload_len_type = 2;
				parser->payload_len = 0;
				UPDATE_STATE(s_ws_frame_payload_len_64_1);
			} else {
				SET_ERRNO(WSE_INVALID_PAYLOAD_LEN);
				return p - data;
			}
			p++;
			break;
			
		case s_ws_frame_payload_len_16_1:
			parser->payload_len = ((uint64_t)ch << 8);
			UPDATE_STATE(s_ws_frame_payload_len_16_2);
			p++;
			break;
			
	case s_ws_frame_payload_len_16_2:
		parser->payload_len |= ch;
		parser->payload_received = 0;
		if (parser->mask) {
			UPDATE_STATE(s_ws_frame_mask_key_1);
		} else {
			/* 没有 mask，清空 mask 相关字段 */
			parser->mask_index = 0;
			memset(parser->masking_key, 0, sizeof(parser->masking_key));
			if (parser->payload_len == 0) {
				UPDATE_STATE(s_ws_frame_complete);
			} else {
				UPDATE_STATE(s_ws_frame_payload);
			}
		}
			p++;
			break;
			
		case s_ws_frame_payload_len_64_1:
			parser->payload_len = ((uint64_t)ch << 56);
			UPDATE_STATE(s_ws_frame_payload_len_64_2);
			p++;
			break;
			
		case s_ws_frame_payload_len_64_2:
			parser->payload_len |= ((uint64_t)ch << 48);
			UPDATE_STATE(s_ws_frame_payload_len_64_3);
			p++;
			break;
			
		case s_ws_frame_payload_len_64_3:
			parser->payload_len |= ((uint64_t)ch << 40);
			UPDATE_STATE(s_ws_frame_payload_len_64_4);
			p++;
			break;
			
		case s_ws_frame_payload_len_64_4:
			parser->payload_len |= ((uint64_t)ch << 32);
			UPDATE_STATE(s_ws_frame_payload_len_64_5);
			p++;
			break;
			
		case s_ws_frame_payload_len_64_5:
			parser->payload_len |= ((uint64_t)ch << 24);
			UPDATE_STATE(s_ws_frame_payload_len_64_6);
			p++;
			break;
			
		case s_ws_frame_payload_len_64_6:
			parser->payload_len |= ((uint64_t)ch << 16);
			UPDATE_STATE(s_ws_frame_payload_len_64_7);
			p++;
			break;
			
		case s_ws_frame_payload_len_64_7:
			parser->payload_len |= ((uint64_t)ch << 8);
			UPDATE_STATE(s_ws_frame_payload_len_64_8);
			p++;
			break;
			
	case s_ws_frame_payload_len_64_8:
		parser->payload_len |= ch;
		parser->payload_received = 0;
		if (parser->mask) {
			UPDATE_STATE(s_ws_frame_mask_key_1);
		} else {
			/* 没有 mask，清空 mask 相关字段 */
			parser->mask_index = 0;
			memset(parser->masking_key, 0, sizeof(parser->masking_key));
			if (parser->payload_len == 0) {
				UPDATE_STATE(s_ws_frame_complete);
			} else {
				UPDATE_STATE(s_ws_frame_payload);
			}
		}
		p++;
		break;
			
		case s_ws_frame_mask_key_1:
			parser->masking_key[0] = ch;
			parser->mask_index = 0;
			UPDATE_STATE(s_ws_frame_mask_key_2);
			p++;
			break;
			
		case s_ws_frame_mask_key_2:
			parser->masking_key[1] = ch;
			UPDATE_STATE(s_ws_frame_mask_key_3);
			p++;
			break;
			
		case s_ws_frame_mask_key_3:
			parser->masking_key[2] = ch;
			UPDATE_STATE(s_ws_frame_mask_key_4);
			p++;
			break;
			
		case s_ws_frame_mask_key_4:
			parser->masking_key[3] = ch;
			parser->payload_received = 0;
			if (parser->payload_len == 0) {
				UPDATE_STATE(s_ws_frame_complete);
			} else {
				UPDATE_STATE(s_ws_frame_payload);
			}
			p++;
			break;
			
	case s_ws_frame_payload:
		{
			size_t remaining = parser->payload_len - parser->payload_received;
			size_t available = end - p;
			size_t to_process = (remaining < available) ? remaining : available;
			
			/* 调用 payload 回调 */
			if (settings->on_frame_payload && to_process > 0) {
				/* 如果有 mask，需要先应用 mask 到临时缓冲区 */
				if (parser->has_mask) {
					uint8_t temp_buf[4096];
					const char *payload_data = p;
					
					/* 如果数据量大，分批处理 */
					size_t processed = 0;
					while (processed < to_process) {
						size_t chunk = (to_process - processed > sizeof(temp_buf)) ? 
						               sizeof(temp_buf) : (to_process - processed);
						
						/* 复制数据到临时缓冲区 */
						memcpy(temp_buf, payload_data + processed, chunk);
						
						/* 应用 mask */
						apply_mask(temp_buf, chunk, parser->masking_key, 
						          parser->mask_index + processed);
						
						/* 调用回调 */
						if (settings->on_frame_payload(parser, (const char *)temp_buf, chunk) != 0) {
							SET_ERRNO(WSE_UNKNOWN);
							return p - data + processed;
						}
						
						processed += chunk;
					}
				} else {
					/* 没有 mask，直接传递数据 */
					if (settings->on_frame_payload(parser, p, to_process) != 0) {
						SET_ERRNO(WSE_UNKNOWN);
						return p - data;
					}
				}
			}
			
			parser->payload_received += to_process;
			if (parser->has_mask) {
				parser->mask_index = (parser->mask_index + to_process) % 4;
			}
			p += to_process;
			
			if (parser->payload_received >= parser->payload_len) {
				UPDATE_STATE(s_ws_frame_complete);
				/* 不要 break，继续执行下一个 case，立即处理 frame_complete */
				/* fall through */
			} else {
				break;
			}
	}
	/* 注意：如果 payload 接收完毕，会 fall through 到 s_ws_frame_complete */
	
	case s_ws_frame_complete:
		/* 检查是否是关闭帧 */
		if (parser->opcode == WS_OPCODE_CLOSE) {
			parser->close_received = 1;
		}
			
			CALLBACK_NOTIFY_(frame_complete, p - data);
			if (WEBSOCKET_PARSER_ERRNO(parser) != WSE_OK) {
				return p - data;
			}
			
			/* 如果收到关闭帧，停止解析 */
			if (parser->close_received) {
				return p - data;
			}
			
		/* 重置状态，准备解析下一个帧 */
		parser->state = s_ws_frame_start;
		parser->fin = 0;
		parser->opcode = 0;
		parser->mask = 0;
		parser->has_mask = 0;
		parser->payload_len = 0;
		parser->payload_received = 0;
		parser->mask_index = 0;
		parser->payload_len_type = 0;
		/* 不增加 p，因为当前字节可能属于下一个帧 */
		/* 继续处理剩余数据（如果有），不使用 break，让循环条件检查 */
		/* 如果 p < end，继续处理下一个帧；否则循环自然结束 */
		continue;
			
		default:
			SET_ERRNO(WSE_INVALID_FRAME);
			return p - data;
		}
	}
	
	return p - data;
}

/* 获取错误名称 */
const char *websocket_errno_name(enum ws_errno err) {
	if (err >= 0 && err < sizeof(ws_strerror_tab) / sizeof(ws_strerror_tab[0])) {
		return ws_strerror_tab[err].name;
	}
	return "WSE_UNKNOWN";
}

/* 获取错误描述 */
const char *websocket_errno_description(enum ws_errno err) {
	if (err >= 0 && err < sizeof(ws_strerror_tab) / sizeof(ws_strerror_tab[0])) {
		return ws_strerror_tab[err].description;
	}
	return "unknown error";
}

/* 获取版本 */
unsigned long websocket_parser_version(void) {
	return WEBSOCKET_PARSER_VERSION_MAJOR * 0x10000 |
	       WEBSOCKET_PARSER_VERSION_MINOR * 0x00100 |
	       WEBSOCKET_PARSER_VERSION_PATCH * 0x00001;
}

/* 获取版本字符串（类似 libuv 的 uv_version_string()） */
const char *ez_websocket_parser_version_string(void) {
	static char version_string[32] = {'\0'};
	
	/* 如果没有格式化过，则格式化 */
	if (version_string[0] == '\0') {
		snprintf(version_string, sizeof(version_string), "%d.%d.%d",
				WEBSOCKET_PARSER_VERSION_MAJOR,
				WEBSOCKET_PARSER_VERSION_MINOR,
				WEBSOCKET_PARSER_VERSION_PATCH);
	}

	return version_string;
}
