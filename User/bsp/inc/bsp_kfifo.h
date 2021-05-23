/*
*********************************************************************************************************
*
*	模块名称 : FIFO驱动模块
*	文件名称 : bsp_kfifo.h
*	说    明 : 头文件
*
*	Copyright (C), 2015-2020, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#ifndef _BSP_KFIFO_H
#define _BSP_KFIFO_H
#include <string.h>
#include <stdint.h>
#include <stddef.h>

/* ring buffer */
typedef struct ringbuffer
{
    uint8_t* buffer_ptr;
    /* use the msb of the {read,write}_index as mirror bit. You can see this as
     * if the buffer adds a virtual mirror and the pointers point either to the
     * normal or to the mirrored buffer. If the write_index has the same value
     * with the read_index, but in a different mirror, the buffer is full.
     * While if the write_index and the read_index are the same and within the
     * same mirror, the buffer is empty. The ASCII art of the ringbuffer is:
     *
     *          mirror = 0                    mirror = 1
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Full
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     *  read_idx-^                   write_idx-^
     *
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Empty
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * read_idx-^ ^-write_idx
     *
     * The tradeoff is we could only use 32KiB of buffer for 16 bit of index.
     * But it should be enough for most of the cases.
     *
     * Ref: http://en.wikipedia.org/wiki/Circular_buffer#Mirroring */
    uint16_t read_mirror : 1;
    uint16_t read_index : 15;
    uint16_t write_mirror : 1;
    uint16_t write_index : 15;
    /* as we use msb of index as mirror bit, the size should be signed and
     * could only be positive. */
    int16_t buffer_size;
}KFIFO_T;

enum ringbuffer_state
{
    RT_KFIFOBUFFER_EMPTY,
    RT_KFIFOBUFFER_FULL,
    /* half full is neither full nor empty */
    RT_KFIFOBUFFER_HALFFULL,
};

/**
 * kfifo for DeviceDriver
 *
 * Please note that the ring buffer implementation of RT-Thread
 * has no thread wait or resume feature.
 */
void bsp_kfifo_init(KFIFO_T* rb, uint8_t* pool, int16_t size);
void bsp_kfifo_reset(KFIFO_T* rb);
size_t bsp_kfifo_put(KFIFO_T* rb, const uint8_t* ptr, uint16_t length);
size_t bsp_kfifo_put_force(KFIFO_T* rb, const uint8_t* ptr, uint16_t length);
size_t bsp_kfifo_putchar(KFIFO_T* rb, const uint8_t ch);
size_t bsp_kfifo_putchar_force(KFIFO_T* rb, const uint8_t ch);
size_t bsp_kfifo_get(KFIFO_T* rb, uint8_t* ptr, uint16_t length);
size_t bsp_kfifo_getchar(KFIFO_T* rb, uint8_t* ch);
size_t bsp_kfifo_data_len(KFIFO_T* rb);
uint16_t bsp_kfifo_get_size(KFIFO_T* rb);

/** return the size of empty space in rb */
#define bsp_kfifo_space_len(rb) ((rb)->buffer_size - bsp_kfifo_data_len(rb))

#endif
