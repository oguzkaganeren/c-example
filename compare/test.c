#include <stdio.h>
#include <string.h>
int main()
{
    char myArray[5][50]={
    "hakkıBilmem",
    ".zyaDayi",
    "muhammedAkpolat",
    "oguzKaganEREN",
    "bilalKim"
    };
    int myInt;
    printf("sayi:");
	scanf("%d", &myInt);
	myInt=myInt-1;
	int i;
	int j;
	int loc=0;
	int locTwo=0;
	char temp[50];
	for(i=0;i<5;i++){
		/*while(myArray[i][myInt+loc]=='.'){
			loc++;
		}*/
			for(j=0;j<5;j++){
				/*while(myArray[j][myInt+locTwo]=='.'){
					locTwo++;
				}*/
					if(myArray[i][myInt+loc]<myArray[j][myInt+locTwo]&&i!=j){
						strcpy(temp,myArray[i]);
						strcpy(myArray[i],myArray[j]);
						strcpy(myArray[j],temp);
					}
				locTwo=0;
			}
		loc=0;
	}
	for(i=0;i<5;i++){
		printf("%s\n",myArray[i]);
	}
    return 0;
}
