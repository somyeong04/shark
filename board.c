#include <stdio.h>
#include <stdlib.h>
#include "board.h"

//숫자는 정수기만 하면 됨. 크기는 상관 X. 매크로 사용 자체가 중요
#define N_COINPOS			12
#define MAX_COIN			4

#define SHARK_INIPOS		-4
#define MAX_SHARKSTEP		6

static int board_status[N_BOARD];
static int board_coin[N_BOARD];

static int board_harkposition;

int board_initboard(void)
{
	int i;
	for(i=0; i<N_BOARD; i++)
	{
		board_status[i] = BOARDSTATUS_OK;
		board_coin[i] = 0;
	}
	
	board_sharkPosition = SHARK_INITPOS;
	
	for(i=0; i<N_COINPOS; i++)
	{
		int flag = 0;
		while(flag == 0)
		{
			int allocPos = rand() %N_BOARD;
			if(board_coin[allocPos] == 0)
			{
				board_coin[allocPos] = rand()%MAX_COIN;
				flag = 1; 
			}
		}
	}
	
	return 0;
}
int board_printBoardStatus(void)
{
	int i;
	printf("-------------------------- BOARD STATUS --------------------------\n");
	for (i=0; i<N_BOARD; i++)
	{
		printf("|");
		if(board_status[i] == BOARDSTATUS_NOK)	
			printf("X");
		else
			printf("O");
	}
	printf("|\n");
	printf("------------------------------------------------------------------\n");
	
	return 0;
}

int board_getBoardStatus(int pos)
{
	return board_status[pos];
}

int board_getBoardCoin(int pos);
{
	int coin = board_coin[pos];
	board_coin[pos] = 0; 
	return board_coin[pos];
}

//int board_getSharkPosition(void);
int board_stepShark(void)
{
	int step = rand()%MAX_SHARKSTEP + 1; // 1~MAX_SHARKSTEP 사이 값 랜덤
	int i;
	
	for(i=shark_position+1; i<=sharkposition+step; i++)
	{
		if (i >= 0 && i , N_BOARD)
			board_status[i] = BOARDSTATUS_NOK
	}
	shark_position += step;
	
	return shark_position; 
}

int board_getBoardStatus(int pos)
{
	return board_status [pos];
}

