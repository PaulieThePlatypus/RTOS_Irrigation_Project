#include "fsm_smart_irrigation.h"

int status = 0;

void fsm_irrigation_init(){
	HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, SET);
	HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, SET);
	HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, SET);
	HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
	HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
	status = T1;
	setTimer(0, 5000);
}

void fsm_irrigation_run(){
	switch(status){
		case T1:
			HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, RESET);
			if(isTimerExpired(0) == 1){
				status = T2;
				setTimer(0, 5000);
				HAL_GPIO_WritePin(SEG1_GPIO_Port, SEG1_Pin, SET);
			}
			break;
		case T2:
			HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, RESET);
			if(isTimerExpired(0) == 1){
				status = T3;
				setTimer(0, 5000);
				HAL_GPIO_WritePin(SEG2_GPIO_Port, SEG2_Pin, SET);
			}
			break;
		case T3:
			HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, RESET);
			if(isTimerExpired(0) == 1){
				status = P1;
				setTimer(0, 10000);
				HAL_GPIO_WritePin(SEG3_GPIO_Port, SEG3_Pin, SET);
			}
			break;
		case P1:
			HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, RESET);
			if(isTimerExpired(0) == 1){
				status = P2;
				setTimer(0, 30000);
				HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, SET);
			}
			break;
		case P2:
			HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, RESET);
			if(isTimerExpired(0) == 1){
				status = IDLE;
				setTimer(0, 60000);
				HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, SET);
			}
			break;
		case IDbLE:
			if(isTimerExpired(0) == 1){
				status = T1;
				setTimer(0, 5000);
			}
			break;
		default:
			break;
	}
}

