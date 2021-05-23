/*
*********************************************************************************************************
*
*	模块名称 : KFIFO驱动
*	文件名称 : bsp_kfifo.c
*	版    本 : V1.8
*	说    明 : 采用串口中断+FIFO模式实现多个串口的同时访问
*	修改记录 :
*		版本号  日期        作者      说明
*		V1.0    2021-02-21  cctv180   正式发布
*
*	Copyright (C), 2015-2030, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/
#include "bsp_kfifo.h"

#define RT_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))
#define RT_ALIGN_SIZE 4

__inline uint16_t bsp_kfifo_get_size(KFIFO_T* rb)
{
	//  ASSERT(rb != RT_NULL);
	return rb->buffer_size;
}

static __inline enum ringbuffer_state bsp_kfifo_status(KFIFO_T* rb)
{
    if (rb->read_index == rb->write_index)
    {
        if (rb->read_mirror == rb->write_mirror)
            return RT_KFIFOBUFFER_EMPTY;
        else
            return RT_KFIFOBUFFER_FULL;
    }
    return RT_KFIFOBUFFER_HALFFULL;
}

void bsp_kfifo_init(KFIFO_T* rb, uint8_t* pool, int16_t size)
{
    //ASSERT(rb != RT_NULL);
   // ASSERT(size > 0);

    /* initialize read and write index */
    rb->read_mirror = rb->read_index = 0;
    rb->write_mirror = rb->write_index = 0;

    /* set buffer pool and size */
    rb->buffer_ptr = pool;
    rb->buffer_size = RT_ALIGN_DOWN(size, RT_ALIGN_SIZE);
}

/**
 * put a block of data into ring buffer
 */
size_t bsp_kfifo_put(KFIFO_T* rb, const uint8_t* ptr, uint16_t length)
{
    uint16_t size;

    // ASSERT(rb != RT_NULL);

     /* whether has enough space */
    size = bsp_kfifo_space_len(rb);

    /* no space */
    if (size == 0)
        return 0;

    /* drop some data */
    if (size < length)
        length = size;

    if (rb->buffer_size - rb->write_index > length)
    {
        /* read_index - write_index = empty space */
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->write_index += length;
        return length;
    }

    memcpy(&rb->buffer_ptr[rb->write_index],
        &ptr[0],
        rb->buffer_size - rb->write_index);
    memcpy(&rb->buffer_ptr[0],
        &ptr[rb->buffer_size - rb->write_index],
        length - (rb->buffer_size - rb->write_index));

    /* we are going into the other side of the mirror */
    rb->write_mirror = ~rb->write_mirror;
    rb->write_index = length - (rb->buffer_size - rb->write_index);

    return length;
}

/**
 * put a block of data into ring buffer
 *
 * When the buffer is full, it will discard the old data.
 */
size_t bsp_kfifo_put_force(KFIFO_T* rb, const uint8_t* ptr, uint16_t length)
{
    uint16_t space_length;

    // ASSERT(rb != RT_NULL);

    space_length = bsp_kfifo_space_len(rb);

    if (length > rb->buffer_size)
    {
        ptr = &ptr[length - rb->buffer_size];
        length = rb->buffer_size;
    }

    if (rb->buffer_size - rb->write_index > length)
    {
        /* read_index - write_index = empty space */
        memcpy(&rb->buffer_ptr[rb->write_index], ptr, length);
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->write_index += length;

        if (length > space_length)
            rb->read_index = rb->write_index;

        return length;
    }

    memcpy(&rb->buffer_ptr[rb->write_index],
        &ptr[0],
        rb->buffer_size - rb->write_index);
    memcpy(&rb->buffer_ptr[0],
        &ptr[rb->buffer_size - rb->write_index],
        length - (rb->buffer_size - rb->write_index));

    /* we are going into the other side of the mirror */
    rb->write_mirror = ~rb->write_mirror;
    rb->write_index = length - (rb->buffer_size - rb->write_index);

    if (length > space_length)
    {
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = rb->write_index;
    }

    return length;
}

/**
 *  get data from ring buffer
 */
size_t bsp_kfifo_get(KFIFO_T* rb, uint8_t* ptr, uint16_t length)
{
    size_t size;

    // ASSERT(rb != RT_NULL);

     /* whether has enough data  */
    size = bsp_kfifo_data_len(rb);

    /* no data */
    if (size == 0)
        return 0;

    /* less data */
    if (size < length)
        length = size;

    if (rb->buffer_size - rb->read_index > length)
    {
        /* copy all of data */
        memcpy(ptr, &rb->buffer_ptr[rb->read_index], length);
        /* this should not cause overflow because there is enough space for
         * length of data in current mirror */
        rb->read_index += length;
        return length;
    }

    memcpy(&ptr[0],
        &rb->buffer_ptr[rb->read_index],
        rb->buffer_size - rb->read_index);
    memcpy(&ptr[rb->buffer_size - rb->read_index],
        &rb->buffer_ptr[0],
        length - (rb->buffer_size - rb->read_index));

    /* we are going into the other side of the mirror */
    rb->read_mirror = ~rb->read_mirror;
    rb->read_index = length - (rb->buffer_size - rb->read_index);

    return length;
}

/**
 * put a character into ring buffer
 */
size_t bsp_kfifo_putchar(KFIFO_T* rb, const uint8_t ch)
{
    //  ASSERT(rb != RT_NULL);

      /* whether has enough space */
    if (!bsp_kfifo_space_len(rb))
        return 0;

    rb->buffer_ptr[rb->write_index] = ch;

    /* flip mirror */
    if (rb->write_index == rb->buffer_size - 1)
    {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = 0;
    }
    else
    {
        rb->write_index++;
    }

    return 1;
}

/**
 * put a character into ring buffer
 *
 * When the buffer is full, it will discard one old data.
 */
size_t bsp_kfifo_putchar_force(KFIFO_T* rb, const uint8_t ch)
{
    enum ringbuffer_state old_state;

    //  ASSERT(rb != RT_NULL);

    old_state = bsp_kfifo_status(rb);

    rb->buffer_ptr[rb->write_index] = ch;

    /* flip mirror */
    if (rb->write_index == rb->buffer_size - 1)
    {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = 0;
        if (old_state == RT_KFIFOBUFFER_FULL)
        {
            rb->read_mirror = ~rb->read_mirror;
            rb->read_index = rb->write_index;
        }
    }
    else
    {
        rb->write_index++;
        if (old_state == RT_KFIFOBUFFER_FULL)
            rb->read_index = rb->write_index;
    }

    return 1;
}

/**
 * get a character from a ringbuffer
 */
size_t bsp_kfifo_getchar(KFIFO_T* rb, uint8_t* ch)
{
    //  ASSERT(rb != RT_NULL);

      /* ringbuffer is empty */
    if (!bsp_kfifo_data_len(rb))
        return 0;

    /* put character */
    *ch = rb->buffer_ptr[rb->read_index];

    if (rb->read_index == rb->buffer_size - 1)
    {
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = 0;
    }
    else
    {
        rb->read_index++;
    }

    return 1;
}

/**
 * get the size of data in rb
 */
size_t bsp_kfifo_data_len(KFIFO_T* rb)
{
    switch (bsp_kfifo_status(rb))
    {
    case RT_KFIFOBUFFER_EMPTY:
        return 0;
    case RT_KFIFOBUFFER_FULL:
        return rb->buffer_size;
    case RT_KFIFOBUFFER_HALFFULL:
    default:
        if (rb->write_index > rb->read_index)
            return rb->write_index - rb->read_index;
        else
            return rb->buffer_size - (rb->read_index - rb->write_index);
    };
}

/**
 * empty the rb
 */
void bsp_kfifo_reset(KFIFO_T* rb)
{
    // ASSERT(rb != RT_NULL);

    rb->read_mirror = 0;
    rb->read_index = 0;
    rb->write_mirror = 0;
    rb->write_index = 0;
}
