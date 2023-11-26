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
	printf("******************************************************************\n");
	printf("******************************************************************\n");
	printf("*************************   GAME START!   ************************\n");
	printf("******************************************************************\n");
	printf("******************************************************************\n");
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
	printf("------------------------------------------------------------------\n\n");
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

int game_end(void)
{
    int i;
    int flag_end = 1;
    // 플레이어가 모두 사망했는지 확인
    for (i = 0; i < N_PLAYER; i++)
    {
        if (player_status[i] == PLAYERSTATUS_LIVE)
        {
            flag_end = 0;  // 살아있는 플레이어가 있다면 flag_end를 0으로 설정
            break;
        }
    }
    
    	return flag_end;
}
   
int getAlivePlayer(void)
{
    int i;
    int cnt = 0;
    for (i=0; i<N_PLAYER; i++)
	{
    	if (player_status[i] == PLAYERSTATUS_END)
         cnt++;
    }
    return cnt;
}
   
int getWinner(void)
{
    int i;
    int winner = 0;
    int max_coin = -1;
      
    for (i=0; i<N_PLAYER; i++)
	{
        if (player_coin[i] > max_coin)
		{
            max_coin = player_coin[i];
            winner = i;
        }
    }
    return winner;
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
			printPlayerPosition(i);
		}
			
		pos += step;
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
        	player_position[turn] = N_BOARD - 1;
        
    	else if (player_position[turn]== N_BOARD - 1)
			player_status[turn] = PLAYERSTATUS_END;
		
    	printf("Die result: %i, Player %s moved to %i!\n", step, player_name[turn], player_position[turn]);

		//2-4. 동전 줍기
		coinResult = board_getBoardCoin(player_position[turn]);
		player_coin[turn] += coinResult;
		
		if(coinResult > 0)
			printf("***************   LUCKY! Player %s got %i coin(s)!   ***************\n", player_name[turn], coinResult);
		
		
		//2-5. 다음 턴 
		turn = (turn + 1) % N_PLAYER; //wrap around, %N_PLAYER없으면 turn>player >> 오류남 
		
		//2-6. if(조건: 모든 플레이어가 한 번씩 턴을 돈 후) 상어 동작
		if (turn % 3 == 0)
		{
			int shar_pos = board_stepShark();
			checkDie();
			printf("****************   WARNING! Shark moved to %i!   *****************\n", shar_pos); // 상어 위치 알림 
		}
	} while(game_end() == 0);
	
	//3. 정리(승자 계산, 출력 등) 
	int winner = getWinner(); // 승자의 인덱스 저장
		int alivePlayers = getAlivePlayer(); // 생존한 플레이어 수 저장

	printf("%d Players are alive. The Winner is %s.\n", alivePlayers, player_name[winner]);
	for (i = 0; i < N_PLAYER; i++)
	{
    	if (player_status[i] == PLAYERSTATUS_LIVE)
		{
        	printf("%s is Alive.\n", player_name[i]);
    	}
	}

return 0;

}
