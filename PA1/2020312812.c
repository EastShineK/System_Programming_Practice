#include<sys/types.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>  // 나중에 지워야함 오류확인용

void ITOA(char *c, int n){
	int rad = 10;
	int count = 0;
	int deg = 1;
	if(n==0){
		*c = '0';
		*(c+1) = '\0';
	}
	else{
		while(1){
			if(n/deg>0)
				count++;
			else
				break;
			deg=deg*rad;
		}
		deg=deg/rad;

		for(int i = 0; i < count; i++){
			*(c+i)=(n/deg)+'0';
			n = n - (n/deg)*deg;
			deg=deg/rad;
		}

		*(c+count)='\0';
	}
}

int disting(char a, char b){
	
	if(a == b)
		return 1;
	else if(a == b + 32)
		return 1;
	else if(a == b - 32)
		return 1;
	else
		return 0;
}

int checkWordStartEnd(char a, char b){
	if(a == 32 && b == 32)
		return 0;
	else if(a == 32 && b == '\0')
		return 0;
	else if(a == '\0' && b == 32)
		return 0;
	else if(a == '\0' && b == '\0')
		return 0;
	else if(a != 32 && b == 32)
                return 1;
        else if(a != 32 && b == '\0')
                return 1;
        else if(a != '\0' && b == 32)
                return 1;
        else if(a != '\0' && b == '\0')
                return 1;

	/*else
		return 1;*/
}

int main(int argc, char **argv){
	int infd;
	char buf[1];
	char buf2[150][100];
	char saveLine[1024];
	char ch;
	int Idx = 0;
	int len, nbytes = 1;
	int number = 1;

//단어 형식:한 단어 위치 찾기 라인넘버&인덱스 출력  clear
//단어 단어 형식: 2개 이상의 단어가 포함되어 있는 라인넘버 출력 순서상관없음
//"단어 단어"형식: 라인넘버&start인덱스
//단어*단어형식: 두 단어 사이에 하나 이상의 단어가 포함되어 있는 라인출력
//대문자 소문자 구분안함

	while(1){
	
		int lineNumber = 1;
		int wordNumber = 0, wordIndex = 0;
		int k = 0;
		int imple = 1, check = 0, NumW;
		for(int i = 0; i < 150; i++){
			for(int j = 0; j < 100; j++){
				buf2[i][j] = 0;
			}
		}

		if((infd = open(argv[1], O_RDONLY)) < 0){
                        perror("open");
                        exit(1);
        	}


		while(read(0, &ch, 1)){ //implementation 입력 받기
			if(ch == '\n'){
				buf2[wordNumber][Idx++] = '\0';
				if(wordNumber > 0)
					NumW = wordNumber + 1;
				Idx = 0;
				wordNumber = 0;
				break;
			}
			else{
				if(ch == 42){//* 
					//printf("hi1\n");
					buf2[wordNumber][Idx++] = '\0';
					Idx = 0;
					wordNumber++;
					imple = 4;
				}
				else if(ch == 32){//' '
					buf2[wordNumber][Idx++] = '\0';
					Idx = 0;
					wordNumber++;
					if(buf2[0][0] != 34)
						imple = 2;
					else
						imple = 3;
					//printf("hi3\n");
				}
				else{
					buf2[wordNumber][Idx++] = ch;
				}				
			}
		}
		//printf("%d ", imple);
		//printf("%s\n", buf2); 
		// check OK
		
		if(wordNumber == 0){
			for(int v = 0; v < 100; v++){
				if(buf2[0][v] == 0){
					len = v;
					break;
				}
			}
		}


		nbytes = 1;

		while(nbytes != 0){		
			//printf("kakakaka\n");
			for(int i = 0; ;i++){
				//printf("jajaj\n");
				nbytes = read(infd, buf, sizeof(buf));
				saveLine[k] = buf[0];			
				k++;
				//printf("qiqiqiqiq\n");
				if(buf[0] == '\n'){
					//printf("vavavav\n");
					saveLine[k-1] = '\0';
					//printf("%d %d %d\n", lineNumber, imple, nbytes);
					if(imple == 1){ // word
						int ans = 0;
						int j = 0;
						for(j ; j < k; j++){
							if(disting(buf2[0][0], saveLine[j])){
								wordIndex = j;
								
								if(j > 0 && saveLine[j-1] != 32  /*saveLine[j-1] != 0*/)
									continue;
								for(int c = 0; c < len; c++){
									if(disting(buf2[0][c], saveLine[j])){
										j++;
										ans = 1;
										//if(lineNumber = 106)
										//	printf("%d %d hi\n", lineNumber, j);
										 if(buf2[0][c+1] == '\0'){
                                                                                             
											 if(saveLine[j] == 32||saveLine[j] == '\0'){
                                                                                                       ans = 1;
                                                                                                       break;
                                                                                               }
                                                                                               else{
                                                                                                       ans = 0;
                                                                                                       break;
                                                                                               }
                                                                                }
		
									}
									else{
										//if(lineNumber = 106)
										//	printf("sorry\n");
										ans = 0;
										break;
										
									}
								}//for c end
							}//if end
							if(ans == 1){
								char Line[5];
								ITOA(Line, lineNumber);
								char Index[3];
								ITOA(Index, wordIndex);
								int lineCopy = lineNumber;
								int wordCopy = wordIndex;
								int sizeL = 1, sizeI = 1;
								while(1){
									lineCopy = lineCopy / 10;
									if(lineCopy > 0)
										sizeL++;
									else 
										break;
								}
								while(1){
                                                                        wordCopy = wordCopy / 10;
                                                                        if(wordCopy > 0)
                                                                                sizeI++;
                                                                        else
                                                                                break;
                                                                }
								write(1,Line, sizeL);
								write(1, ":", 1);
								write(1, Index, sizeI);
								write(1, " ", 1);
								ans = 0;
								break;
							}
						} // for j end					
					}
					else if(imple == 2){ // word word
		
						int x = 0;
						int ans = 0;
						int count = 0, ansCount = 0;
						int t;
						int wLength;
						char word[50];
						int countIdx = 0;
						//printf("%d come in\n", lineNumber);
						//if(lineNumber = 6)
							//printf("%d\n", NumW);
						for(int i = 0; i < NumW; i++){
							int existingAnsCount = ansCount;
							for(int q = 0; q < 100; q++){
								if(buf2[i][q] == 0){
									wLength = q;
									break;
								}
							}
							for(int a = 0; a < 49; a++){
								word[a] = 0;
							}
							//if(lineNumber = 88)
                                                        //	printf("%d\n", wLength);
							while(1){

							if(saveLine[x] != '\0' && saveLine[x] != 32){
								word[countIdx++] = saveLine[x];
								x++;
							}
							else{
								word[countIdx++] = saveLine[x];
								x++;
								int k = 0;
								//if(lineNumber == 88)
									//printf("%s %s\n", buf2[i], word);
								while(disting(buf2[i][k], word[k])){
									k = k + 1;
									//if(lineNumber == 88 && wLength == k)
                                                                        	//printf("%c\n", word[k]);
									if(wLength == k && (word[k] == ' ' || word[k] == '\0')){
										//if(lineNumber == 6)
											//printf("%d\n", ansCount);
										ansCount++;
										for(int a = 0; a < 49; a++){
                        			                                        word[a] = 0;
			                                                        }

										//printf("hi\n");
										break;
									}
								}
								
								if(ansCount == existingAnsCount + 1){
									//if(lineNumber == 6)
                                                                                //printf("hihi22\n");
									x = 0;
									break;
								}
								else{
									countIdx = 0;
									for(int a = 0; a < 49; a++){
                		                                                word[a] = 0;
		                                                        }
								}
								if(saveLine[x-1] == '\0'){
									//if(lineNumber == 6)
										//printf("hihi22\n");
									break;
								}
							}// else end 
							} //while 1 end
							countIdx = 0;
							x = 0;
						} //for NumW end

						//if(lineNumber == 88)
							//printf("k :%d %d\n", NumW, ansCount);
						//printf("%d come in 474\n", lineNumber);	
						if(NumW == ansCount){
							char Line[5];
                                                        ITOA(Line, lineNumber);
							int lineCopy = lineNumber;
                                                        int sizeL = 1;
                                                        while(1){
                                                                lineCopy = lineCopy / 10;
                                                                if(lineCopy > 0)
                                                                        sizeL++;
                                                                else
                                                                        break;
                                                        }
							write(1,Line, sizeL);
							write(1," ", 1);
							//break;
						}//단어의 갯수와 실제 있는 단어의 갯수 비교
						//count = 0;
						ansCount = 0;		
						//printf("%d come in 84645\n", lineNumber);
					}//imple 2 endl
					else if(imple == 3){ // word*word
						
					}
					else if(imple == 4){ // double quotation and word

					}
					//printf("korea\n");
					
					k = 0;
					wordIndex = 0;
					lineNumber++;
					break;
				}//if buf[n] == '\n'
				//printf("h2222i\n");
				
			}//for end
			//printf("cocococo\n");
		} // while nbytes check end
		//printf("lalalal\n");
		//inp_size = 0;
		//printf("\n");
		write(1, "\n", 1);
	}//while 1 end
	
	close(infd);

	return 0;
}
