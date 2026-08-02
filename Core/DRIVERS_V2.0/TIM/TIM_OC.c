#include "TIM_OC.h"


inline static volatile uint32_t* pick_ccmr(TIM_TypeDef *tim, uint8_t chn){
	if(chn==1 || chn==2) 			return &tim->CCMR1;
	else if(chn==3 || chn==4) 		return &tim->CCMR2;
	else while(1);
}

inline static uint8_t ccmr_shift(uint8_t chn){
	if(chn==1 || chn==3) 			return 0;
	else if(chn==2 || chn==4) 		return 8;
	else while(1);
}


inline static void ccrx_val(TIM_TypeDef *tim,uint8_t chn,uint16_t val){
	if(chn==1) 		tim->CCR1=val;
	else if(chn==2) tim->CCR2=val;
	else if(chn==3) tim->CCR3=val;
	else if(chn==4) tim->CCR4=val;
}

inline static uint8_t ccer_shift(uint8_t chn){
	if(chn==1) 			return 0;
	else if(chn==2) 	return 4;
	else if(chn==3)		return 8;
	else if(chn==4)     return 12;
	else while(1);
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

	volatile uint32_t *CCMRx=pick_ccmr(tim, channel);
	uint8_t shift_ccmr=ccmr_shift(channel);

	*CCMRx&=~(((3<<0)|(1<<2)|(7<<4))<<shift_ccmr);
	*CCMRx|=((config->fast_enable<<2)|(config->timer_mode<<4))<<shift_ccmr;

	uint8_t shift_ccer=ccer_shift(channel);

	tim->CCER&=~(((1<<0)|(1<<1))<<shift_ccer);
	tim->CCER|=((1<<0)|(config->polarity<<1))<<shift_ccer;
}
