#include <stdbool.h> // 包含布尔类型定义
#include <stddef.h> // 包含size_t类型定义
#include <stdint.h> // 包含固定宽度整数类型定义

/* 检查编译器是否针对错误的目标操作系统。 */
#if defined(__linux__)
#error "你没有使用交叉编译器，极有可能会遇到问题"
#endif

/* 仅适用于32位ix86目标。 */
#if !defined(__i386__)
#error "需要使用ix86-elf编译器编译"
#endif

/* 这个枚举定义了 VGA 文本模式下的 16 种标准颜色 */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

/* 这里计算颜色*/
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

#define VGA_WIDTH   80 // VGA文本模式的标准分辨率是80列
#define VGA_HEIGHT  25 // VGA文本模式的标准分辨率是25行
#define VGA_MEMORY  0xB8000

size_t terminal_row; // 行坐标
size_t terminal_column; // 列坐标
uint8_t terminal_color; // 当前文本颜色，包含前景色和背景色
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY; // VGA文本模式的显存地址，指向一个16位的数组，每个元素表示一个字符和它的颜色属性

// 初始化终端，设置初始行列坐标和颜色，并清空屏幕
void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

// 设置当前文本颜色
void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

// 在指定位置输出一个字符，参数c是要输出的字符，color是字符的颜色，x和y是字符在屏幕上的列和行坐标
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

 /* 这里是处理换行和字符输出 参数 c: 要输出的字符 */
void terminal_putchar(char c)
{
    if (c == '\n') { // 如果字符是换行符
        terminal_column = 0; // 列坐标重置为0，回到行首
        if (++terminal_row == VGA_HEIGHT)
            terminal_row = 0;
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row); //调用字符输出函数
        if (++terminal_column == VGA_WIDTH) { // 如果达到屏幕宽度换行
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT) // 如果达到屏幕高度回到顶部
                terminal_row = 0;
        }
    }
}


// 输出一个字符串，参数data是字符串的指针，size是字符串的长度
void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++) // 逐个字符输出字符串
		terminal_putchar(data[i]);
}

// 输出一个以null结尾的字符串，参数data是字符串的指针
void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}

// 内核的入口点，初始化终端并输出欢迎信息
void kernel_main(void) 
{
	/* 初始化终端接口 */
	terminal_initialize();

	/* 换行支持留作练习(已解决) */
	terminal_writestring("Hello, OwO-kernel World!\n");
}