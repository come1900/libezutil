#ifndef EZ_WEBSOCKET_PARSER_H
#define EZ_WEBSOCKET_PARSER_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/* WebSocket 解析器版本 */
#define WEBSOCKET_PARSER_VERSION_MAJOR 1
#define WEBSOCKET_PARSER_VERSION_MINOR 0
#define WEBSOCKET_PARSER_VERSION_PATCH 0

/**
 * WebSocket Protocol Magic String (RFC 6455 Section 4.2.2)
 * 
 * This is a fixed GUID string defined in the WebSocket protocol specification
 * (RFC 6455) used during the handshake process to compute the
 * Sec-WebSocket-Accept header value.
 * 
 * The server must concatenate the client's Sec-WebSocket-Key value with this
 * magic string, compute the SHA-1 hash of the concatenated string, and then
 * base64-encode the hash to produce the Sec-WebSocket-Accept response header.
 * 
 * This magic string ensures that:
 * 1. The server correctly implements the WebSocket protocol
 * 2. The connection is not being hijacked by a non-WebSocket server
 * 3. The handshake process follows the standard protocol specification
 * 
 * Reference: RFC 6455, Section 4.2.2 - Sending the Server's Opening Handshake
 */
#define WEBSOCKET_MAGIC_STRING "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
/*
 * WebSocket 帧头部最大额外字节数：
 *   - 2 字节基本头（FIN/RSV/Opcode + MASK/PayloadLen）
 *   - 8 字节扩展长度（当 payload >= 2^16 时使用）
 *   - 4 字节 Masking-key（客户端发送必须携带）
 */
#define WS_MAX_FRAME_OVERHEAD 14


typedef struct websocket_parser websocket_parser;
typedef struct websocket_parser_settings websocket_parser_settings;

/* WebSocket 帧操作码 */
enum ws_opcode {
	WS_OPCODE_CONTINUATION = 0x0,
	WS_OPCODE_TEXT         = 0x1,
	WS_OPCODE_BINARY       = 0x2,
	WS_OPCODE_CLOSE        = 0x8,
	WS_OPCODE_PING         = 0x9,
	WS_OPCODE_PONG         = 0xA
};

/* WebSocket 解析器错误码 */
enum ws_errno {
	WSE_OK = 0,
	WSE_INVALID_FRAME,
	WSE_INVALID_OPCODE,
	WSE_INVALID_PAYLOAD_LEN,
	WSE_INVALID_MASK,
	WSE_INCOMPLETE_FRAME,
	WSE_PAYLOAD_TOO_LARGE,
	WSE_UNKNOWN
};

/* 回调函数类型 */
typedef int (*ws_data_cb)(websocket_parser *parser, const char *at, size_t length);
typedef int (*ws_cb)(websocket_parser *parser);

/* WebSocket 解析器结构 */
struct websocket_parser {
	/* PRIVATE */
	unsigned int state : 8;
	unsigned int opcode : 4;
	unsigned int fin : 1;
	unsigned int mask : 1;
	unsigned int has_mask : 1;
	unsigned int payload_len_type : 2; /* 0: <126, 1: 126, 2: 127 */
	
	uint64_t payload_len;
	uint64_t payload_received;
	uint8_t masking_key[4];
	uint8_t mask_index;
	
	uint32_t nread;
	
	/* READ-ONLY */
	unsigned int ws_errno : 4;
	unsigned int close_received : 1; /* 收到关闭帧标志 */
	
	/* PUBLIC */
	void *data; /* 用户数据指针 */
};

/* WebSocket 解析器设置（回调函数集合） */
struct websocket_parser_settings {
	ws_cb      on_frame_begin;      /* 帧开始 */
	ws_data_cb on_frame_payload;    /* 帧负载数据 */
	ws_cb      on_frame_complete;    /* 帧完成 */
};

/* 初始化解析器 */
void websocket_parser_init(websocket_parser *parser);

/* 初始化设置 */
void websocket_parser_settings_init(websocket_parser_settings *settings);

/* 执行解析，返回已解析的字节数 */
size_t websocket_parser_execute(websocket_parser *parser,
                                const websocket_parser_settings *settings,
                                const char *data,
                                size_t len);

/* 获取错误码 */
#define WEBSOCKET_PARSER_ERRNO(p) ((enum ws_errno)(p)->ws_errno)

/* 获取错误名称 */
const char *websocket_errno_name(enum ws_errno err);

/* 获取错误描述 */
const char *websocket_errno_description(enum ws_errno err);

/* 获取版本 */
unsigned long websocket_parser_version(void);

/* 获取版本字符串（类似 libuv 的 uv_version_string()） */
const char *ez_websocket_parser_version_string(void);

#ifdef __cplusplus
}
#endif
#endif

