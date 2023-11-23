#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "board.h"

#define MAX_CHARNAME 200
#define MAX_DIE			6
#define N_PLAYER		3

#define PLAYERSTATUS_LIVE		0
#define PLAYERSTATUS_DIE		1
#define PLAYERSTATUS_END		2 //숫자는 서로 다르기만하면 상관 없음 

int player_position[N_PLAYER];
char player_name[N_PLAYER][MAX_CHARNAME];
int player_coin [N_PLAYER];
int player_status[N_PLAYER];
char player_statusString[3][MAX_CHARNAME] = {"LIVE", "DIE", "END"};

//if (player_status[0] = PLAYERSTATUS_LIVE ) "0번 플레이어가 살아 있다면" 조건문 

void opening(void)
{
	printf("*******************************************************\n");
	printf("*******************************************************\n");
	printf("*******************   GAME START!   *******************\n");
	printf("*******************************************************\n");
	printf("*******************************************************\n");
}

int rolldie(void)
{
	return rand()%MAX_DIE+1;
}

void printPlayerPosition (int player)
{
	int i;
	for(i=0; i<N_BOARD; i++)
	{
		printf ("|");
		if (i == player_position[player])
			printf("%c", player_name[player][0]);
		else
		{
			if (board_getBoardStatus(i) == BOARDSTATUS_NOK)
				printf("X");
			else
				printf(" ");
		}
	}
	printf("|\n");
}

void printPlayerStatus(void)
{
	int i;
	for (i=0; i<N_PLAYER; i++)
	{
		printf("%s: pos %i, coin %i, status %i\n", player_name[i], player_position[i], player_coin[i], player_status[i]);
	}
	printf("-----------------------\n");
}

void checkDie(void)
{
	int i;
	for (i=0; i<N_PLAYER; i++)
	{
		if (board_getBoardStatus(player_position[i]) == BOARDSTATUS_NOK)
		player_status[i] = PLAYERSTATUS_DIE;
	}
}
 
int main(int argc, char *argv[])
{
	int pos = 0;
	int i;
	int turn; 
	
	srand((unsigned) time(NULL));
	
	//0. opening
	opening();
	
	//1. 초기화 & 플레이어 이름 결정
	board_initboard();
	//1-2.
	for (i=0; i<N_PLAYER; i++) //N_PLAYER 대신 숫자를 넣으면 N_PLAYER 수가 바뀔 경우 오류가 남 
	{
		player_position[i] = 0;
		player_coin[i] = 0;
		player_status[i] = PLAYERSTATUS_LIVE;
		//player_name
		printf("Player %i's name is: ", i);
		scanf("%s", player_name[i]); // player_name 앞에는 & 필요없음 
	}
	 
	//2.반복문(플레이어 턴) 
	do 
	
	{
	
		int step = rolldie();
		int coinResult;
		char c;
		
		if (player_status[turn] != PLAYERSTATUS_LIVE)
		{
			turn = (turn + 1) % N_PLAYER;
			continue;
		}
		
		
		//2-1. 보드 & 플레이어의 상태 출력
		board_printBoardStatus();
		for (i=0; i<N_PLAYER; i++)
		{
			printPlayerPosition (i);
		}
			
		pos += step;
		
		coinResult = board_getBoardCoin(pos);
		printf("Press any key to continue:");
    	scanf("%d", &c);
    	fflush(stdin);
      
    	printPlayerStatus();

		//2-2. 주사위 던지기
		printf("%s turn! \n", player_name[turn]);
		printf("Press any key to roll a die: ");
		scanf("%d", &c);
		fflush(stdin);
		
		step = rolldie();
		
		//2-3. 이동
		player_position[turn] += step;
		
      	if (player_position[turn] >= N_BOARD)
		{
        	player_position[turn] = N_BOARD-1;
        
    	}
    	if (player_position[turn]== N_BOARD - 1)
			player_status[turn] = PLAYERSTATUS_END;
    	//printf()

		//2-4. 동전 줍기
		coinResult = board_getBoardCoin(pos);
		player_coin[turn] += coinResult;
		
		//2-5. 다음 턴 
		turn = (turn + 1) % N_PLAYER; //wrap around, %N_PLAYER없으면 turn>player >> 오류남 
		
		//2-6. if(조건: 모든 플레이어가 한 번씩 턴을 돈 후) 
			//상어 동작
			int shar_pos = board_stepShark();
			
			checkDie();
	} while(1);
	//3. 정리(승자 계산, 출력 등) 
		
}
