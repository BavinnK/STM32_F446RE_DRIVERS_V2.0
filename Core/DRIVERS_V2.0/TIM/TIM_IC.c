#include "TIM_IC.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//INLINE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

inline static  volatile uint32_t* pick_ccmr(TIM_TypeDef *tim, uint8_t chn){
	if(chn==1 || chn==2) 		return &tim->CCMR1;
	else if(chn==3 || chn==4) 	return &tim->CCMR2;
	else return (void*) 0;
}

inline static uint8_t ccmr_shift(uint8_t chn){
	if(chn==1 || chn==3) 		return 0;
	else if(chn==2 || chn==4) 	return 8;
	else while(1);
}

inline static uint8_t ccer(TIM_TypeDef *tim,uint8_t chn){
	if(chn==1) return 0;
	else if(chn==2) return 4;
	else if(chn==3) return 8;
	else if(chn==4) return 12;
	else while(1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//END INLINE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//TIMER INPUT CAPTURE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////

void TIMx_IC_init(TIM_TypeDef *tim,tim_ic_config_t *config, uint8_t channel){
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

	*CCMRx&=~(((3<<0)|(3<<2)|(0b1111<<4))<<shift_ccmr);
	*CCMRx|=((1<<0)|(config->input_capture_pcs<<2)|(config->input_capture_filter<<4))<<shift_ccmr;

	uint8_t shift_ccer=ccer(tim, channel);

	uint8_t polarity_bits=0;
	if(polarity_bits & 0b01) polarity_bits|=(1<<1);
	if(polarity_bits & 0b10) polarity_bits|=(1<<3);

	tim->CCER&=~(((1<<0)|(1<<1)|(1<<3))<<shift_ccer);
	tim->CCER|=((1<<0)|polarity_bits)<<shift_ccer;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//END TIMER INPUT CAPTURE FUNCTIONS
///////////////////////////////////////////////////////////////////////////////////////////////////
