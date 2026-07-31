#include "TIM_OC.h"
volatile static uint32_t *CCMRx;
static uint8_t shift;
inline static void pick_ccmr_shift(TIM_TypeDef *tim, uint8_t chn){
	if(chn==1 || chn==2){
		CCMRx=&tim->CCMR1;
	}
	else if(chn==3 || chn==4){
		CCMRx=&tim->CCMR2;
	}

	if(chn==1 || chn==3){
		shift=0;
	}
	else if(chn==2 || chn==4){
		shift=8;
	}
}



void TIMx_OC_init(TIM_TypeDef *tim,uint8_t channel,tim_oc_config_t *config){
	tim_config_t basic_config={
			.TIM_DIR=config->dir,
			.TIM_PCS=config->pcs,
			.TIM_ARR=config->arr
	};
	TIMx_init(tim, &basic_config);

	tim->CR1&=~(3<<5);
	tim->CR1|=(config->center_align_mode<<5);


	*CCMRx&=~(((3<<0)|(1<<2)|(7<<4))<<shift);

	*CCMRx|=((config->fast_enable<<2)|(config->timer_mode<<4))<<shift;

}
