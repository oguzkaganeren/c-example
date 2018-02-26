#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
struct _Record {
	char name[35];
	char surname[35];
	char address[120];
	char note[250];
	int deleted;
};
typedef struct _Record Record;
struct _Index {
	char nameSurname[70];
	int adr;
};
typedef struct _Index Index;
typedef struct nodeInd {
    Index val;
    struct nodeInd * next;
} node_Ind;
void lower_string(char str[]) {
   int c = 0;
 
   while (str[c] != '\0') {
      if (str[c] >= 'A' && str[c] <= 'Z') {
         str[c] = str[c] + 32;
      }
      c++;
   }
}

Record getFileData(int adr)
{
	FILE* inputFile;
    	inputFile = fopen("data.dat","rb");
    	Record rc;
    	if(inputFile) {
    		fseek(inputFile, sizeof(int)+adr*sizeof(rc), SEEK_SET );
		  	fread(&(rc),sizeof(rc),1, inputFile);
        	fclose(inputFile);
  		  }
  		  
		
	return rc;
	
	    
}
int getRecordCount(){
	FILE* inputFile;
    	inputFile = fopen("data.dat","r");
    	Record rc;
    	int sz=0;
    	if(inputFile) {
		  	fseek(inputFile, 0, SEEK_END);
			sz = ftell(inputFile);
        	fclose(inputFile);
  		  }
	return (sz-4)/444;
}
int print_list(node_Ind * head) {
    node_Ind * current = head->next;
	Record rc;
	int counter=0;
	printf("No|Name\tSurname\tAddress\tNote\n");
    while (current != NULL) {
        	rc=getFileData(current->val.adr);
        		counter++;
        		printf("%d.",counter);
        		printf("%s ",rc.name);
    			printf("%s\t",rc.surname);
				printf("%s\t",rc.address);
				printf("%s\n",rc.note);
    		
        current = current->next;
    }
    return counter;
}
int addNodeBottomInd(Record rec, node_Ind *head,int generationNew){
	int repeat=0;//bir ise veri tekrarı vardır
    //create new node(temp)
    node_Ind *newNode = (node_Ind*)malloc(sizeof(node_Ind));

    if(newNode == NULL){
        fprintf(stderr, "Unable to allocate memory for new node\n");
        exit(-1);
    }
    node_Ind *tempCount = head;
    int counter;
    if(generationNew==-1){
		counter=getRecordCount();
    }else{
    	counter=generationNew;//yeniden index oluşturuluyorsa buraya girer
    }
    
    char namesur[70];
    strcpy(namesur,rec.name);
    strcat(namesur,rec.surname);
    strcpy(newNode->val.nameSurname ,namesur);
    newNode->val.adr=counter;
    newNode->next = NULL;
    //check for first insertion
    if(head->next == NULL){
        head->next = newNode;
    }
    else
    {
        //else loop through the list and find the last
        //node, insert next to it
        node_Ind *current = head;
        node_Ind *prev=NULL;
        int isOkay=0;
        
        while (current->next != NULL) {
        	if (strcmp(current->val.nameSurname,newNode->val.nameSurname)==0)
        	{
        		repeat=1;
        		break;
        	}
        	else if (strcmp(current->val.nameSurname,newNode->val.nameSurname)>0)
        	{
        		newNode->next=current;
      			prev->next = newNode;
      		    isOkay=1;
        		break;
        	} 
        	prev=current;
            current = current->next;
        };
        if (isOkay==0&&repeat==0)
        {
        	//veri tekrarını onlemek icin karşılaştırma yapıyorum
        	if (strcmp(current->val.nameSurname,newNode->val.nameSurname)>0)
        	{
        		newNode->next=current;
      		 	 prev->next = newNode;
      			  isOkay=1;
        	}
        	else if(strcmp(current->val.nameSurname,newNode->val.nameSurname)==0){	
        		repeat=1;
        	}else{
        		current->next=newNode;
        	}
        }
        
    }
    return repeat;
}
void addBottomRec(Record rec)
{
	FILE* fp;
	fp = fopen("data.dat", "rb+");
	if(!fp){
		fp = fopen("data.dat", "wb+");
		fclose(fp);
	}
	fp = fopen("data.dat", "rb+");
	fseek( fp, 0, SEEK_SET );
	int e=0;
	fwrite(&e, 1, sizeof(int), fp);
	fseek( fp, 0, SEEK_END );
    fwrite(&(rec), 1, sizeof(rec), fp);
	fclose(fp);
}
void generateChanged()
{
	FILE* fp;
	fp = fopen("data.dat", "rb+");
	if(!fp)
		return;
	fseek( fp, 0, SEEK_SET );
	int e=1;
	fwrite(&e, 1, sizeof(int), fp);
	
	fclose(fp);
	
	    
}
int searchRecord(node_Ind *headInd){
	char str[70];
	char *camStr;
	printf("\nPlease any character or word\n");
    scanf("%s",str);
    node_Ind * current = headInd;
	Record rc;
	int flag=1;
	int counter=0;
	printf("No|Name\tSurname\tAddress\tNote\n");
    while (current != NULL) {
    	flag=1;
    	if (strlen(current->val.nameSurname)!=0)
    	{
    		int i = 0;
    		for (i = 0; i < strlen(str); ++i)
    		{

    			if (current->val.nameSurname[i]!=str[i])
    			{
    				flag=0;
    			}
    		}
    		if (flag==1)
    		{
    			counter++;
    			printf("%d.",counter);
    			rc=getFileData(current->val.adr);
    			if (rc.deleted==0)
    			{
    				printf("%s ",rc.name);
	    			printf("%s\t",rc.surname);
					printf("%s\t",rc.address);
					printf("%s\n",rc.note);
    			}
	    		
    		}
        
    }
    
        current = current->next;
    }
    return counter;

}
int setFileData(int adr,Record rec){
	FILE* inputFile;
    	inputFile = fopen("data.dat","rb+");
    	Record rc=rec;
    	if(inputFile) {
    		fseek(inputFile, sizeof(int)+adr*sizeof(rc), SEEK_SET );
		  	fwrite(&(rc), 1, sizeof(rc), inputFile);
        	fclose(inputFile);
  		  }
}
int updateRecord(node_Ind *headInd){
	//0 dönerse sıkıntı yoktur
	node_Ind *currInd=headInd;
    int count=searchRecord(headInd);
    if (count!=0)
    {
    	int option=0;
    	do
    	{
    		printf("\nPlease select a option\n");
    		scanf("%d",&option);
    	} while (!(option>0&&option<=count));
    	currInd=headInd;
    	option-=1;
    	int i = 0;
    	for (i = 0; i < option; i++)
    	{
    		currInd = currInd->next;
    	}
    	int valueCount=0;
    	do
    	{
    		printf("Please select the value to be changed\n");
    		printf("1-Name\n2-Surname\n3-Address\n4-Note");
    		printf("\nPlease select a option\n");
    		scanf("%d",&valueCount);
    	} while (!(valueCount>0&&valueCount<=4));
    	Record rc=getFileData(currInd->val.adr);
    	Record temp;
    	switch(valueCount){
    		case 1:
    		printf("Current name:");
    		printf("%s\n",rc.name );
    		printf("Enter the value you want to change:\n");
    		scanf("%s",temp.name);
    		break;
    		case 2:
    		printf("Current surname:");
    		printf("%s\n",rc.surname );
    		printf("Enter the value you want to change:\n");
    		scanf("%s",temp.surname);
    		break;
    		case 3:
    		printf("Current address:");
    		printf("%s\n",rc.address);
    		printf("Enter the value you want to change:\n");
    		scanf("%s",temp.address);
    		break;
    		case 4:
    		printf("Current note:");
    		printf("%s\n",rc.note);
    		printf("Enter the value you want to change:\n");
    		scanf("%s",temp.note);
    		break;
    	}
    	

    	
    	
    	
    	char namesur[70];
   		 strcpy(namesur,temp.name);
   	 	strcat(namesur,temp.surname);
   	 	const int tempAdr=currInd->val.adr;
   	 	while(currInd != NULL){
   	 		if (strcmp(currInd->val.nameSurname,namesur)==0&&currInd->val.adr!=tempAdr)
	        	{
	        		//veri tekrarı kontrolü
	        		return 1;
	        	}
   	 		currInd = currInd->next;
   	 	}
   	 	currInd=headInd;
   	 	i=0;
   	 	for (i; i < option; i++)
    	{
    		currInd = currInd->next;
    	}
    	strcpy(currInd->val.nameSurname,namesur);
    		switch(valueCount){
    		case 1:
    		strcpy(rc.name,temp.name);
    		break;
    		case 2:
    		strcpy(rc.surname,temp.surname);
    		break;
    		case 3:
    		strcpy(rc.address,temp.address);
    		break;
    		case 4:
    		strcpy(rc.note,temp.note);
    		break;
    	}

    		setFileData(tempAdr,rc);
    		return 0;
    }
    else{
    	return 1;
    }
}
int deleteRecord(node_Ind *headInd){
	node_Ind *currInd=headInd;
	node_Ind *prev;
    int k=print_list(headInd);
     if (k!=0)
    {
    	int option=0;
    	do
    	{
    		printf("\nChoose what you want to delete\n");
    		scanf("%d",&option);
    	} while (!(option>0&&option<=k));
    	currInd=headInd;
    	k=0;
    	int i = 0;
    	for (i = 0; i < option; i++)
    	{
    		prev=currInd;
    		currInd = currInd->next;
    	}
    	Record rc=getFileData(currInd->val.adr);
    	rc.deleted=1;
   		if (prev==NULL)
	        		{
	        			currInd->next=NULL;
	        			setFileData(currInd->val.adr,rc);
	        			return 0;
	        		}
	        		else{
	        			prev->next=currInd->next;
	        			setFileData(currInd->val.adr,rc);
	        			return 0;
	        		}


    }else{
    	return 1;
    }
}
void generateFileIndex(node_Ind *headInd)
{
	FILE* fp;
	fp = fopen("index.dat", "wb");
	if(!fp)
		return;
	node_Ind *current = headInd;
        while (current->next != NULL) {
        	fwrite(&(current->next->val), 1, sizeof(current->next->val), fp);
            current = current->next;
        };
	fclose(fp);
	
	    
}

int readFileIndex(node_Ind *headInd)
{
	int error=1;
	FILE* inputFile;	
    	inputFile = fopen("index.dat","rb");
    	 if(!inputFile) {
    	 	return error;
  		  }
  		  error=0;
    		fseek(inputFile, SEEK_SET,0);
			Index dx;	
	  			fread(&(dx),sizeof(dx),1, inputFile);
	  			while(!feof(inputFile)) {
		  	 	//addNodeBottomIndex(dx,headInd);
	  			//printf("%s\n",((Record*)dx.adr)->name);
      		  fread(&(dx),sizeof(dx),1, inputFile);
   			 }
        		
      		  fclose(inputFile);
	    return error;
}
void generateNewIndex(node_Ind *index_list)
{
	FILE* inputFile;
    	inputFile = fopen("data.dat","rb");
    	Record rc;
    	if(inputFile) {
    		fseek(inputFile, sizeof(int), SEEK_SET );
    		int count=0;
    		fread(&(rc),sizeof(rc),1, inputFile);
    		while(!feof(inputFile)) { 
    			addNodeBottomInd(rc,index_list,count);
        		fread(&(rc),sizeof(rc),1, inputFile);
        		count++;
   			 }
		  	
        	fclose(inputFile);
  		  }
  		  
		
	
	    
}
int readFileDataError()
{
	FILE* inputFile;
	int error=1;
    	inputFile = fopen("data.dat","rb");
    	if(!inputFile) {
        	
  		  }
  		  else{
  		  	fseek(inputFile, 0, SEEK_SET );
		  	fread(&(error),sizeof(error),1, inputFile);
       		 fclose(inputFile);
  		  }
		
	
		return error;
	    
}
int main(int argc, char **argv)
{
	node_Ind *index_list= malloc(sizeof(node_Ind));
	int hasChanged=0;
	if (readFileDataError()==1||readFileIndex(index_list)==1)//exit yapmadan çıktıysa veya index dosyası yoksa
	{	
		generateNewIndex(index_list);
	}
	else{
		readFileIndex(index_list);
	}
	Record rec;
	int option;
	do
	{
		printf("\n\nMy Address Book Application\n");
		printf("===============================\n");
		printf("1. Add New Record\n");
		printf("2. Update a Record\n");
		printf("3. Delete a Record\n");
		printf("4. Find a Record\n");
		printf("5. List Records\n");
		printf("6. Exit\n");
		printf("Enter your choice==>");
		scanf("%d",&option);
		switch(option){
			case 1:
				printf("Please enter a name:\n");
				scanf("%s",rec.name);
				lower_string(rec.name);
				printf("Please enter a surname:\n");
				scanf("%s",rec.surname);
				lower_string(rec.surname);
				printf("Please enter a address:\n");
				scanf("%s",rec.address);
				lower_string(rec.address);
				printf("Please enter a note:\n");
				scanf("%s",rec.note);
				rec.deleted=0;
				if(addNodeBottomInd(rec,index_list,-1)==0)//1 geldiyse veri tekrarı vardır
				{
					if (hasChanged==0)
						{
						generateChanged();
						hasChanged=1;
						}
					addBottomRec(rec);
					printf("Added new record\n");
				}else{
					printf("The data already exists in your index");
				}
				

				
			break;
			case 2:
			
			if (updateRecord(index_list)==0)
			{
				if (hasChanged==0)
						{
						generateChanged();
						hasChanged=1;
						}
				printf("The data have updated\n");
			}else{
				printf("There is a problem(maybe the data already exists)\n");
			}
			
			break;
			case 3:
			
			if (deleteRecord(index_list)==0)
			{
				if (hasChanged==0)
						{
						generateChanged();
						hasChanged=1;
						}
				printf("The data have deleted\n");
			}
			break;
			case 4:
				searchRecord(index_list);
			break;
			case 5:
			print_list(index_list);
			break;

		}
	} while (option!=6);
	
	generateFileIndex(index_list);

}

