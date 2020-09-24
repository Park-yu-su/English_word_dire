#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h> //리눅스에서 sleep 함수 있는 곳 

typedef struct word{ //word 링크드리스트
	struct word *next;
	char english[16];
	char korean[3][100];
}word;

word *head; //head 노드를 자주 쓰기 때문에 전역 포인터로 둠

void input_link(FILE *p,int number, int *len_node){ //txt 파일 값을 링크드리스트에 넣기
	char txtfile[500]; //txt 파일의 한 줄 통째로 받는 문자열 배열
	char *txtsplit[10] = {NULL, }; // 통째로 받는 문자열을 split
	char filename[30]; //오픈할 파일 이름 변수
	*len_node = 0; //초기화
	word *curr = head; //노드 위치 확인용
	sprintf(filename,"%d.txt",number); //찾을 txt파일 이름 만들기
	p = fopen(filename,"r"); //파일을 읽기 모드로 열기 
	if(p==NULL){ // 존재하지 않는 파일 열 시 강종
		printf("존재하지 않는 파일입니다.\n");
		return;
	}

	word *re_check = head; //새로운 파일 열기 전에 링크드리스트 초기화
	while(re_check->next!=NULL){
		word *remove = re_check->next;
		strcpy(remove->english,"\0");
		strcpy(remove->korean[0],"\0");
		strcpy(remove->korean[1],"\0");
		strcpy(remove->korean[2],"\0");
		re_check->next = remove->next;
		free(remove);
	}
	

	for(int i=0; i<=30; i++){ //1일 최대 30개 단어
		int j=0; //split 위치 지정 변수
		fgets(txtfile, sizeof(txtfile),p); //txtfile 변수에 한 줄 통째로
		
		if(txtfile[strlen(txtfile)-1]=='\n')
			txtfile[strlen(txtfile)-1] = '\0'; //개행문자 제거	

		char *split = strtok(txtfile," "); //공백 기준으로 split (임시 저장)
		while(split!=NULL){
			txtsplit[j] = split;
			j++;
			split = strtok(NULL," ");
		} //이 과정이 끝나면 txtsplit에는 영단어 해석 NULL 이렇게 들어간다
		
				
		word *newNode = malloc(sizeof(word)); //영단어 넣을 새 노드 동적 할당
		strcpy(newNode->english,txtsplit[0]);
		for(int i=1; i<10; i++){ //txtsplit 만큼 일단 반복
			if(txtsplit[i]!=NULL && i<4) //최대 해석 3개 들어가니까 규제
				strcpy(newNode->korean[i-1],txtsplit[i]);
			    txtsplit[i]= '\0'; // \0을 통해 초기화
		}
			
		curr = head;
		while(curr->next!=NULL) //맨 뒤에 영단어를 순서대로 추가
			curr = curr->next;  //반복문을 통해 맨 뒤에 NULL값이 나올 때까지 이동
		
		if(strcmp(curr->english,newNode->english)==0){ //똑같은 단어 계속 받는 거 막기 위함
			strcpy(newNode->english,"\0");
			free(newNode); //겹치면 확실하게 제거
			return;
		}
		curr->next = newNode; 
		newNode->next = NULL;
		*len_node+=1; //노드개수추가

}
	free(curr); 
		
}

void show_word(){ //단어장 보기 함수 (링크드리스트 단어 읽기)
	printf("\n\n-----단어장-----------\n");
	word *curr = head->next; 
	while(curr!=NULL){
		printf("%s %s %s %s\n",curr->english,curr->korean[0],curr->korean[1],curr->korean[2]);
		curr = curr->next;
	}
	free(curr);
}

void add_newfile(FILE *p){
	printf(">>영어 단어 암기 프로그램 : 단어장 관리 : 새 파일 추가<<\n");
	char txtfile[100]; //list.txt 파일 받기 (띄어쓰기 포함 50까지 가능)
	char *add; //추가할 파일 번호(임시)
	int add_number; //추가할 파일 번호
	p = fopen("list.txt","r+"); //읽는 모드로 열기
	fscanf(p,"%[^\n]",txtfile);
	char *split = strtok(txtfile, " ");
	while(split!=NULL){
		add = split;
		split = strtok(NULL," ");
	}
	add_number = atoi(add) + 1;  //위 과정을 통해 list에서 다음에 추가할 파일 이름 얻기
	fclose(p); //닫기


	char filename[30]; //마지막 파일명+1.txt가 들어갈 변수
	char txtinput[500]; //입력할 영어 해석*3이 들어갈 변수
	sprintf(filename,"%d.txt",add_number);
	p = fopen(filename, "w"); //마지막 파일명+1.txt을 생성(무조건 새 파일이니 처음부터 쓴다)
	int check = 0; //최대 20개의 단어 입력 확인 변수
	while(check <=20){
		getchar(); //enter 제거
		scanf("%[^\n]",txtinput);
		if(strcmp(txtinput,".add")==0) //만약 .add 입력시 종료
			break;
		fputs(txtinput, p);
		fputs("\n",p); // 한 단어 작성하고 밑으로 내리기
		check ++;
	}// 위 과정을 통해 새로운 메모장에 단어 입력 완료
	fclose(p); //닫기
	char plus_list[10]; //list에 추가할 문자열
	p = fopen("list.txt","a"); //list 뒤에 연결
	sprintf(plus_list, " %d", add_number);
	fputs(plus_list,p); 
	fclose(p);
}

void add_newtext(FILE *p,int filenumber){ //filenumber는 단어 추가할 파일
	printf(">>영어 단어 암기 프로그램 : 단어장 관리 : 새 단어 추가<<\n");
	char filename[30]; //단어 추가할 파일명 들어갈 공간
	char txtinput[500]; //영어 해석*3 들어갈 문자
	sprintf(filename,"%d.txt",filenumber);


	p = fopen(filename,"a+"); 
	int check = 0; //최대 20개


	while(check<=20){
		getchar();
		scanf("%[^\n]",txtinput);
		if(strcmp(txtinput,".add")==0)
			break;
		fputs(txtinput,p);
		fputs("\n",p); //한 단어 쓰고 enter
		check++;
	}
	fclose(p);
	return;
}

void show_list(FILE *p){ //단어 파일 목록 출력 함수
	printf("\n\n----- 단어 파일 목록 -----------\n");
	char txtfile[100]; //list.txt 파일 받기
	char *add; //파일 번호
	char filename[10]; //번호.txt가 들어갈 부분
	int check = 0; //5개씩 입력하고 \n 판단할 변수
	p = fopen("list.txt","r+");
	fscanf(p,"%[^\n]",txtfile);
	char *split = strtok(txtfile," ");
	while(split!=NULL){
		add = split;
		sprintf(filename, "%s.txt", add);
		printf("%6s  ",filename);
		check ++;
		if(check>4){ //한 줄에 5개씩 단어 받고 개행
			printf("\n");
			check = 0;
		}
		split = strtok(NULL," ");	
	}
	printf("\n");
	fclose(p);
}

void node_sort(int *len_node){ //링크드리스트에 넣은 영단어를 정렬(버블 소트)
	word *check = head->next;
	char temp_eg[20], temp_kr[3][100];
		for(int i=0; i<*len_node-2; i++){
			check = head->next;
			for(int j=0; j<*len_node-1-i; j++){
				if(strcmp(check->english,check->next->english)>=0){
					
					strcpy(temp_eg,check->english); //임시 변수에 저장
					strcpy(temp_kr[0],check->korean[0]);
					strcpy(temp_kr[1],check->korean[1]);
					strcpy(temp_kr[2],check->korean[2]);

					strcpy(check->english,check->next->english);
					strcpy(check->korean[0], check->next->korean[0]);
					strcpy(check->korean[1], check->next->korean[1]);
					strcpy(check->korean[2], check->next->korean[2]);
					//만약 정렬 시 오류 생기면 /0 처리를 해줘야 하나

					strcpy(check->next->english,temp_eg);
					strcpy(check->next->korean[0],temp_kr[0]);
					strcpy(check->next->korean[1],temp_kr[1]);
					strcpy(check->next->korean[2],temp_kr[2]);
				}
				check = check->next;
			}
		}
}


int main(){

	head = malloc(sizeof(word)); //위의 전역포인터 head에 동적 할당
	FILE *fp = NULL; //txt 파일을 읽을 포인터
	head->next = NULL;
	int len_node = 0; //input_link와 node_sort 함수에 사용될 변수
	srand(time(NULL));

	fp = fopen("list.txt","r");
	char ex[100]; //최대 50.txt
	fscanf(fp,"%[^\n]",ex);
	fclose(fp);
	fp = fopen("list.txt","w");
	fputs(ex,fp);
	fclose(fp);
	//최초에 0 1 2 3.txt를 외부에서 입력받을 시 밑의 줄에서 입력받아서 코드가 정상적으로 작동되지 않기 때문에 일부러 다시 넣는 과정을 삽입했다.

	
	int main_select; // 단어장 행동 변수

MAIN:
	system("clear");
	printf("-----------------------------------------\n");
	printf(">>영어 단어 암기 프로그램<<\n");
	printf("0. 영어 타자 연습\n");
	printf("1. 영어 단어 맞추기  2. 플래쉬카드\n");
	printf("3. 행맨(hangman)     4. 단어장관리\n");
	printf("5. 프로그램 종료\n\n"); //메뉴
	
	printf("번호를 골라주세요: ");
	scanf("%d",&main_select); //메뉴에서 번호 고르기
	if(main_select == 1) //고른 번호로 이동 goto 함수로 번호마다 프로그램
		goto ONE;
	else if(main_select ==2) //플래쉬카드
		goto TWO;
	else if(main_select ==3) //행맨
		goto THREE;
	else if(main_select ==4) //단어장 관리
		goto FOUR;
	else if(main_select ==5){ //종료
		printf(">>프로그램 종료<<\n");
		system("clear");
		return 0;
	}
	else if(main_select == 0){
		goto ZERO;
	}
	else{
		printf("\n올바르지 않은 수를 입력했습니다. 다시 입력하세요.\n");
		goto MAIN;
	}

ZERO:
{		
	
	int correcteg_filenum; //파일 번호
	printf("파일 번호를 선택하세요: ");
	scanf("%d",&correcteg_filenum);
    input_link(fp,correcteg_filenum,&len_node); //링크드리스트에 삽입 완료
	char input_eg[30];
	int play_check = 1;
	double process = 0, correct_record = 100; 
	int	correct = 0, incorrect = 0;
	time_t start = time(NULL); //시작 시간	i
	word*check = head->next; //링크드리스트에 있는 단어를 출력할 포인터 생성
	
	
	
	while(play_check <= len_node+1){
		system("clear");
		printf(">>영어 단어 암기 프로그램 : 타자연습<<\n");
		printf("\n\n");
		printf("----------------------------------------\n");
		printf("진행도: %3.1f  정확도: %3.1f   오타 수 : %d\n",process,correct_record, incorrect);	
		printf("----------------------------------------\n");
		if(play_check == len_node+1){ //다 입력 완료 (결과)
			break;
		}
		else{
		printf("\n\n\n               제시어 : %s\n",check->english);
		printf("\n\n입력 : ");
		scanf("%s",input_eg);
	
		if(strcmp(check->english,input_eg)==0)
			correct +=1;
		else
			incorrect +=1;
		//계산 : process correct_record
		process += 100/(double)len_node;
		correct_record = ((double)correct/(correct+incorrect))*100;
		play_check +=1;
		check = check->next;
		}
	}
	time_t end = time(NULL); //끝 시간
	getchar();
	printf("\n\n엔터를 누르시면 결과가 출력됩니다...\n");
	while(1){
		char get = getchar();
		if(get == '\n')
			break;
	}
	system("clear");
	printf(">>영어 단어 암기 프로그램 : 타자연습 결과창<<\n\n");
	printf("총 입력한 단어 수 : %d\n",len_node);
	printf("정확도 : %.1f\n",correct_record);
	printf("걸린 시간 : %.1f\n",((double)(end-start)));
	printf("\n---------- 평가 ----------\n");
	if(correct_record>=95)
		printf("축하드립니다. 당신의 정확도 평가는 A+ 입니다.\n");
	else if(correct_record>=90)
		printf("축하드립니다. 당신의 정확도 평가는 A 입니다.\n");
	else if(correct_record>=80)
		printf("당신의 정확도 평가는 B 입니다.\n");
	else 
		printf("당신의 정확도 평가는 C 입니다. 노력하시기 바랍니다.\n");
	
	double check_time = (double)(end-start);
	if(check_time <= 1.5*len_node) //각 단어당 평균 입력 시간 1.5초 이하
		printf("축하드립니다. 당신의 속도 평가는 A+ 입니다.\n");
	else if(check_time <= 2*len_node) // 각 단어당 평균 입력 시간 2초 이하
		printf("축하드립니다. 당신의 속도 평가는 A 입니다.\n");
	else if(check_time <= 3*len_node) // 각 단어당 평균 입력 시간 3초 이하
		printf("당신의 속도 평가는 B 입니다.\n");
	else 
		printf("당신의 속도 평가는 C 입니다. 노력하시기 바랍니다.\n");
	
	while(1){
		char get = getchar();
		if(get == '\n')
			break;
	}
	goto MAIN;

}



ONE:
{
	int correcteg_filenum; //파일 번호
	int correcteg_printnum; //방식
	int random, check_rand_find = 0;//랜덤 시 사용할 변수
	int correct=0 , incorrect=0;

	printf("파일 번호를 선택하세요: ");
	scanf("%d",&correcteg_filenum);
	printf("출력방식을 선택하세요(알파벳 순서대로: 1, 무작위: 2) : ");
    scanf("%d",&correcteg_printnum);
    input_link(fp,correcteg_filenum,&len_node); //링크드리스트에 삽입 완료

	if(correcteg_printnum==1) //알파벳순서 시 정렬 끝
		node_sort(&len_node);
	else if(correcteg_printnum==2) //노드 랜덤
		check_rand_find = 1;

	system("clear");
	printf(">>영어 단어 암기 프로그램 : 영어 단어 맞추기<<\n");
	word*check = head->next; //링크드리스트에 있는 단어를 출력할 포인터 생성
	while(1){ //포인터가 마지막 노드를 가리킬 때까지 반복
		char input_eg[20], print_kr[3][100], print_eg[20];

		for(int i=0; i<3; i++) //이전 노드에 남은 해석 초기화
			strcpy(print_kr[i],"\0");
	
		if(check_rand_find ==1){ //랜덤일 경우 특수하게 변경
			random = rand()%len_node; //0~len_node-1 까지의 수
			for(int i=0; i<random; i++)
				check = check->next; //랜덤의 위치로 이동시킨다
		}

		if(check==NULL) //만약 노드가 맨 마지막이면 처음으로 이동
			check = head->next;
		
		
		for(int i=0; i<3; i++)
			strcpy(print_kr[i] , check -> korean[i]);
		strcpy(print_eg , check -> english);
	  	for(int i=0; i<3 && print_kr[i]!=NULL; i++)
			printf("%s ",print_kr[i]);
		printf("-> ");
		scanf("%s",input_eg);


		if(strcmp(print_eg,input_eg)==0){  //맞은 거
	    	printf("correct!\n");
			correct++;
	  }
	  	else if(strcmp(input_eg,".quit")==0){ //나가기
	 		break;
	  }
	  	else{ //틀린거
			printf("incorrect!\n");
	   		incorrect++;
	  }
		if(check_rand_find==0)
			check = check->next;
		else if(check_rand_find==1)
			check = head->next; //다시 처음으로

    }
	double score;
	score = 100*((double)correct/(correct + incorrect));
	printf("\n\n당신의 점수는 %.2f점입니다.\n",score);
	
	getchar();
	while(1){
		char get = getchar();
		if(get == '\n')
			goto MAIN;
	}
}


TWO:
{
	int correcteg_filenum; //파일 번호
	int correcteg_printnum; //방식
	double speed;
	int random, check_rand_flash = 0;//랜덤 시 사용할 변수
	int correct=0 , incorrect=0;

	printf("속도(초)를 선택하세요: ");
	scanf("%lf",&speed);
	printf("파일 번호를 선택하세요: ");
	scanf("%d",&correcteg_filenum);
	printf("출력방식을 선택하세요(알파벳 순서대로: 1, 무작위: 2) : ");
    scanf("%d",&correcteg_printnum);
    input_link(fp,correcteg_filenum,&len_node); //링크드리스트에 삽입 완료
	
	if(correcteg_printnum==1) //알파벳순서 시 정렬 끝
		node_sort(&len_node);
	else if(correcteg_printnum==2) //노드 랜덤
		check_rand_flash = 1;

//경우에 따라 플래쉬 카드 발동 조건 바꾸기	
	word *check = head->next;
	if(check_rand_flash==1){ //랜덤으로 하는 경우
	while(len_node>0){ //기존의 방법과 다른 경우로 제어를 해야 한다.(노드 X) 복잡
		int go_rand = rand()%len_node;
		for(int i=0; i<go_rand-1; i++)
			check = check->next; //랜덤으로 가기
		system("clear");
		printf(">>영어 단어 암기 프로그램 : 플래쉬카드<<\n");
		printf("\n\n\n\n\n\n");
		if(len_node <=1){ //노드가 하나만 남으면  next는 null이기 때문에 오류난다.
			printf("%35s   ",check->english); 
			for(int i=0; i<3 && check->korean[i]!=NULL; i++)
				printf("%s   ",check->korean[i]);
			printf("\n");
		}
		else{
			printf("%35s   ",check->next->english);
			for(int i=0; i<3 && check->next->korean[i]!=NULL; i++)
				printf("%s   ",check->next->korean[i]);
			printf("\n");
		}
		if(len_node>1){ //노드가 1일 때에는 할 필요가 없다.
		len_node -=1; //노드를 지우기 때문에 노드 개수를 1개씩 감소
		word *remove = check->next; //노드를 끊어야 하기 때문에 랜덤 -1 하고 check->next의 것을 해당 단어로 잡고 출력 뒤 삭제한다.
		check->next = check->next->next;
		free(remove);
		check = head->next;
		}
		else{ //노드가 1개 이하인 경우에는 노드만 줄여서 while문을 끊자.
			len_node -=1; 
		}
		usleep(speed*1000000); //speed초 만큼 딜레이	
		}
	}

	else{ //정렬로 하는 경우
	while(check!=NULL){
		system("clear");
		printf(">>영어 단어 암기 프로그램 : 플래쉬카드<<\n");
		printf("\n\n\n\n\n\n");
		printf("%35s   ",check->english);
		for(int i=0; i<3 && check->korean[i]!=NULL; i++)
			printf("%s   ",check->korean[i]);
		printf("\n");
		usleep(speed*1000000); //speed초 만큼 딜레이	
		
		check = check->next;
		}
	}

	goto MAIN;
}

THREE:
{	
	int correcteg_filenum, random_word;
	printf("파일 번호를 입력하세요: ");	
	scanf("%d",&correcteg_filenum);
	input_link(fp,correcteg_filenum,&len_node); //링크드리스트 삽입

	word *check = head->next;
	random_word = rand()%len_node; //.txt 파일에서 랜덤으로 행맨에 쓸 단어 고르기
	for(int i=0; i<random_word; i++)
		check = check->next;
// 사전 준비 완료 (게임 스타트)
	int life = 6, answer_check = 0, try = 1,  word_len; //목숨, 답 체크, 시도, 영어 길이
	word_len = strlen(check->english);

	char answer[word_len+1]; //답이 들어있는 배열
	char print_an[word_len+1]; //화면에 출력되는배열
	char input_an[10]; //답을 입력하는 문자열 변수
	answer[word_len] = '\0';
	print_an[word_len] = '\0';

	for(int i=0; i<word_len; i++)
		print_an[i] =  '_';
	strcpy(answer,check->english);
	
	while(life>=0){
		system("clear");
		printf(">>영어 단어 암기 프로그램 : 행맨<<\n");
		printf("(힌트) : ");
		for(int i=0; i<3; i++)
			printf("%s ",check->korean[i]);
		printf("\n\n\n");
		printf("--------------------+\n"); //총 21글자
		answer_check = 0; //초기화
		switch(life)
		{
		case 6:
			printf("\n\n\n\n\n");
			break;
		
		case 5:
			printf("                    O\n");
			printf("\n\n\n\n");
			break;
	
		case 4:
			printf("                    O\n");
			printf("                  /\n");
			printf("\n\n\n");
			break;
		case 3:
			printf("                    O\n");
			printf("                  / |\n");
			printf("\n\n\n");
			break;
		case 2:
			printf("                    O\n");
			printf("                  / | \\\n");
			printf("\n\n\n");
			break;
		case 1:
			printf("                    O\n");
			printf("                  / | \\\n");
			printf("                   /\n");
			printf("\n\n");
			break;
		default:
			printf("                    O\n");
			printf("                  / | \\\n");
			printf("                   / \\\n");
			printf("\n\n");
			break;
		}
		
		for(int i=0; i<word_len; i++) //화면에 단어 출력
			printf("%c ",print_an[i]);
		printf("\n\n");

		if(life<=0){ //즉 default를 출력하고 목숨이 0개일 때(단어 입력 전)
			printf("-------------\n");
			printf("| GAME OVER |\n");
			printf("-------------\n");
			break;
		}

		printf("%d번째 시도: ",try);
		getchar(); //enter 입력 방지
		scanf("%s",input_an);
        if(strlen(input_an)>1){
		   printf("\n한 글자만 입력하세요\n2초간 정지됩니다.\n");
		   answer_check = 0;
		   usleep(2000000);
		   continue;

		}

		for(int i=0; i<word_len; i++){
			if(answer[i] == input_an[0]){ //만약 입력한 단어가 답에 있다.
				answer_check = 1; //목숨 부지
				print_an[i] = input_an[0];
			} 
		}
		if(strcmp(answer,print_an)==0){ //만약 최종적으로 정답이면	
			printf("----------------------\n");
			printf("| Congratulations!!! |\n");
			printf("----------------------\n");
			break;
		}

		if(answer_check != 1){ //없는 경우
			life -=1; //목숨 감소
		}
		try++;


	}	
	
	getchar();
	while(1){
		char get = getchar();
		if(get == '\n')
			goto MAIN;
	}
}

FOUR: //단어장 관리
	printf("\n\n------------------------------------------------------\n");
	printf(">>영어 단어 암기 프로그램: 단어장관리<<\n");
	printf("1. 새 파일 추가하기   2. 새 단어 추가하기\n");
	printf("3. 단어장 보기        4. 단어 파일 목록보기\n");
	printf("5. 단어장 관리 종료\n\n");
	
	int num_select;
	printf("번호를 입력하세요: ");
	scanf("%d",&num_select);
	if(num_select ==1){
		add_newfile(fp); //새 파일 추가하기
		system("clear");
	}
	else if(num_select ==2){
		int file;
		printf("파일명(일차): ");
		scanf("%d",&file);
		add_newtext(fp,file);
		system("clear");
	}
	
	else if(num_select ==3){
		int file;
		printf("파일명(일차): ");
		scanf("%d",&file);
		input_link(fp,file,&len_node);
		show_word();
	}
	
	else if(num_select ==4){
		show_list(fp);
	}	
	
	else if(num_select ==5)
		goto MAIN;
	
	else{
		printf("\n잘못된 번호를 입력헀습니다. 다시 입력하세요.\n");
		goto FOUR;
	}

	goto FOUR; //단어장 관리 메뉴로

	return 0;
}	
