#ifndef DMA_H
#define DMA_H

#include "stdint.h"
#include "stm32f446xx.h"

typedef enum{
	SINGLE=0,
	INCR4,
	INCR8,
	INCR16
}m_p_burst;

typedef enum{
	LOW=0,
	MED,
	HIGH,
	VHIGH
}pir_level;

typedef enum{
	BYTE=0,
	HALF_WORD,
	WORD
}data_size;

typedef enum{
	PER_TO_MEM=0,
	MEM_TO_PER,
	MEM_TO_MEM
}data_trasnfer_dir;

typedef enum{
	NORMAL=0,
	CIRCULAR
}dma_mode;

typedef struct{
	DMA_Stream_TypeDef *stream;
	uint8_t channel;
	m_p_burst p_burst;
	m_p_burst m_burst;
	pir_level priority;
	data_size m_data_size;
	data_size p_data_size;
	data_trasnfer_dir data_transfer_dir;
	uint16_t length;
	volatile uint32_t *per_addr;
	uint16_t *mem_addr;
	dma_mode mode;
}
dma_config_t;

void DMAx_init(DMA_TypeDef *DMAx  ,dma_config_t *dma);

#endif
