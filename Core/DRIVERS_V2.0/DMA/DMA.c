#include "DMA.h"

inline static void dma_config(DMA_TypeDef *dma){
	if(dma==DMA1) RCC->AHB1ENR|=(1<<21);
	else if(dma==DMA2) RCC->AHB1ENR|=(1<<22);
}

static inline void clear_flags(DMA_Stream_TypeDef *stream){
	if(stream==DMA1_Stream0) DMA1->LIFCR|=(1<<0)|(0b1111<<2);
	else if(stream==DMA1_Stream1) DMA1->LIFCR|=(1<<6)|(0b1111<<8);
	else if(stream==DMA1_Stream2) DMA1->LIFCR|=(1<<16)|(0b1111<<18);
	else if(stream==DMA1_Stream3) DMA1->LIFCR|=(1<<22)|(0b1111<<24);
	else if(stream==DMA1_Stream4) DMA1->HIFCR|=(1<<0)|(0b1111<<2);
	else if(stream==DMA1_Stream5) DMA1->HIFCR|=(1<<6)|(0b1111<<8);
	else if(stream==DMA1_Stream6) DMA1->HIFCR|=(1<<16)|(0b1111<<18);
	else if(stream==DMA1_Stream7) DMA1->HIFCR|=(1<<22)|(0b1111<<24);
	else if(stream==DMA2_Stream0) DMA2->LIFCR|=(1<<0)|(0b1111<<2);
	else if(stream==DMA2_Stream1) DMA2->LIFCR|=(1<<6)|(0b1111<<8);
	else if(stream==DMA2_Stream2) DMA2->LIFCR|=(1<<16)|(0b1111<<18);
	else if(stream==DMA2_Stream3) DMA2->LIFCR|=(1<<22)|(0b1111<<24);
	else if(stream==DMA2_Stream4) DMA2->HIFCR|=(1<<0)|(0b1111<<2);
	else if(stream==DMA2_Stream5) DMA2->HIFCR|=(1<<6)|(0b1111<<8);
	else if(stream==DMA2_Stream6) DMA2->HIFCR|=(1<<16)|(0b1111<<18);
	else if(stream==DMA2_Stream7) DMA2->HIFCR|=(1<<22)|(0b1111<<24);
}

void DMAx_init(DMA_TypeDef *DMAx  ,dma_config_t *dma){
	dma_config(DMAx);

	dma->stream->CR&=~((7<<25)|(3<<23)|(3<<21)|(3<<16)|(3<<13)|(3<<11)|(1<<8)|(3<<6)|(1<<0));
	dma->stream->CR|=(dma->channel<<25)|
				(dma->m_burst<<23)|
				(dma->p_burst<<21)|
				(dma->priority<<16)|
				(dma->m_data_size<<13)|
				(dma->p_data_size<<11)|
				(dma->mode<<8)|
				(dma->data_transfer_dir<<6);

	clear_flags(dma->stream);
	dma->stream->NDTR=dma->length;

	dma->stream->PAR=(uint32_t)dma->per_addr;
	dma->stream->M0AR=(uint32_t)dma->mem_addr;
}
