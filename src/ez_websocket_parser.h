#ifndef EZ_WEBSOCKET_PARSER_H
#define EZ_WEBSOCKET_PARSER_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

/* WebSocket 解析器版本 */
#define EZ_WEBSOCKET_PARSER_VERSION_MAJOR 1
#define EZ_WEBSOCKET_PARSER_VERSION_MINOR 0
#define EZ_WEBSOCKET_PARSER_VERSION_PATCH 0

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
#define EZ_WEBSOCKET_MAGIC_STRING "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
/*
 * WebSocket 帧头部最大额外字节数：
 *   - 2 字节基本头（FIN/RSV/Opcode + MASK/PayloadLen）
 *   - 8 字节扩展长度（当 payload >= 2^16 时使用）
 *   - 4 字节 Masking-key（客户端发送必须携带）
 */
#define EZ_WS_MAX_FRAME_OVERHEAD 14


typedef struct ez_websocket_parser ez_websocket_parser;
typedef struct ez_websocket_parser_settings ez_websocket_parser_settings;

/* WebSocket 帧操作码 */
enum ez_ws_opcode {
	EZ_WS_OPCODE_CONTINUATION = 0x0,
	EZ_WS_OPCODE_TEXT         = 0x1,
	EZ_WS_OPCODE_BINARY       = 0x2,
	EZ_WS_OPCODE_CLOSE        = 0x8,
	EZ_WS_OPCODE_PING         = 0x9,
	EZ_WS_OPCODE_PONG         = 0xA
};

/* WebSocket 解析器错误码 */
enum ez_ws_errno {
	EZ_WSE_OK = 0,
	EZ_WSE_INVALID_FRAME,
	EZ_WSE_INVALID_OPCODE,
	EZ_WSE_INVALID_PAYLOAD_LEN,
	EZ_WSE_INVALID_MASK,
	EZ_WSE_INCOMPLETE_FRAME,
	EZ_WSE_PAYLOAD_TOO_LARGE,
	EZ_WSE_UNKNOWN
};

/* 回调函数类型 */
typedef int (*ez_ws_data_cb)(ez_websocket_parser *parser, const char *at, size_t length);
typedef int (*ez_ws_cb)(ez_websocket_parser *parser);

/* WebSocket 解析器结构 */
struct ez_websocket_parser {
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
struct ez_websocket_parser_settings {
	ez_ws_cb      on_frame_begin;      /* 帧开始 */
	ez_ws_data_cb on_frame_payload;    /* 帧负载数据 */
	ez_ws_cb      on_frame_complete;    /* 帧完成 */
};

/* 初始化解析器 */
void ez_websocket_parser_init(ez_websocket_parser *parser);

/* 初始化设置 */
void ez_websocket_parser_settings_init(ez_websocket_parser_settings *settings);

/* 执行解析，返回已解析的字节数 */
size_t ez_websocket_parser_execute(ez_websocket_parser *parser,
                                const ez_websocket_parser_settings *settings,
                                const char *data,
                                size_t len);

/* 获取错误码 */
#define EZ_WEBSOCKET_PARSER_ERRNO(p) ((enum ez_ws_errno)(p)->ws_errno)

/* 获取错误名称 */
const char *ez_websocket_errno_name(enum ez_ws_errno err);

/* 获取错误描述 */
const char *ez_websocket_errno_description(enum ez_ws_errno err);

/* 获取版本 */
unsigned long ez_websocket_parser_version(void);

/* 获取版本字符串（类似 libuv 的 uv_version_string()） */
const char *ez_websocket_parser_version_string(void);

#ifdef __cplusplus
}
#endif
#endif

