#include "TIM_OC.h"
volatile static uint32_t *CCMRx;
static uint8_t shift,shift_en;
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

inline static void ccr(TIM_TypeDef *tim,uint8_t chn,uint16_t val){
	if(chn==1){
		tim->CCR1=val;
		shift_en=0;
	}
	else if(chn==2){
		tim->CCR2=val;
		shift_en=4;
	}
	else if(chn==3){
		tim->CCR3=val;
		shift_en=8;
	}
	else if(chn==4){
		tim->CCR4=val;
		shift_en=12;
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

	pick_ccmr_shift(tim, channel);

	*CCMRx&=~(((3<<0)|(1<<2)|(7<<4))<<shift);
	*CCMRx|=((config->fast_enable<<2)|(config->timer_mode<<4))<<shift;

	ccr(tim, channel, config->ccrx_val);

	tim->CCER&=~(((1<<0)|(1<<1))<<shift_en);
	tim->CCER|=((1<<0)|(config->polarity<<1))<<shift_en;



}
