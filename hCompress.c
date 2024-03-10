	#include "hCompress.h"
	#include <string.h>
	#include <stdlib.h>
	#include <stdio.h>
	#include <stdbool.h>

	

	int main(int argc, char *argv[]) {
	   if (argc != 3) {
	      printf("Error: The correct format is \"hcompress -e filename\" or \"hcompress -d filename.huf\"\n"); fflush(stdout);
	    exit(1);
	  }
	  struct tnode* leafNodes = createFreqTable("decind.txt");
	  struct tnode* treeRoot = createHuffmanTree(leafNodes);

	  if (strcmp(argv[1], "-e") == 0) {
	    encodeFile(argv[2], leafNodes);
	  } else { 
	    decodeFile(argv[2], treeRoot);
	  }
	}

	tnode* createFreqTable(char* filename){
	  FILE* file = fopen(filename, "r");
	  tnode* arrHist = (tnode*)malloc(128*sizeof(tnode));
	  for(int i = 0; i<128; i++){
	    (arrHist+i)->c = i;
	    (arrHist+i)->freqCount = 0;
	  }
	  int temp = fgetc(file);
	  while(temp != -1){
	    (arrHist+temp)->freqCount++;
	    temp = fgetc(file);
	  }
	  return arrHist;
	}

	
	tnode* createHuffmanTree(tnode* leafNode){
	  LinkedList* lls = llCreate();
	 
	  for (int p = 0; p < 128; p++) {
	   list_add_in_order(&lls, &leafNode[p]);
	   	
	  }


	  
	  while(lls->next != NULL){

	      tnode* parent = (tnode*)malloc(1*sizeof(tnode));
	      tnode* first = removeFirst(&lls);
	      tnode* second = removeFirst(&lls);
	      int sum = ((first->freqCount)+(second->freqCount));
	      parent->c = '*';
	      parent->freqCount = sum;
	      parent->left = first;
	      parent->right = second;
	      first->parent = parent;
	      second->parent = parent;

	      list_add_in_order(&lls, parent);
	  }

	 
	  tnode* root = removeFirst(&lls);
	  return root;
	}


	void encodeFile(char* filename, tnode* leafNode){
	  FILE* file = fopen(filename, "r");
	  FILE* hufFile = fopen("decind.huf", "w");
	  int temp = fgetc(file);
	  int currBits[60];
	  int bit = 0;

	  
	  unsigned char byte = 0;
	  while(temp != EOF){
	    tnode* curr = &leafNode[temp];
	    int count = 0;
	    while(curr->parent != NULL){
	      if(curr->parent != NULL && curr->parent->left == curr){
			currBits[count] = 0;
	      }
	      if(curr->parent != NULL && curr->parent->right == curr){
			currBits[count] = 1;
		
	      }
	      count++;
	      curr = curr->parent;      
	    }
	    
	    for(int i=count-1; i >= 0; i--)
	      {
	      
	      	byte |= currBits[i] << (7-bit);
	      	bit++;
	      
	      
	      if(bit == 8){
			fputc(byte, hufFile);
			bit = 0;
			byte = 0;
	      }
	      
	    }
	    
	   
	    temp = fgetc(file);
	    count = 0;
	  } 

	  
	   


	}

	void decodeFile(char* filename, tnode* treeRoot){
	  tnode* temp = treeRoot;
	  FILE* file = fopen(filename,"r");
	  char byte = fgetc(file);
	  char currentBit = 0;
	  int bit = 0;
	   FILE* newFile = fopen("decind.huf.txt", "w");
	   fseek(file, 0, SEEK_END);
	   int byteCount = ftell(file);
	   fseek(file, 0, SEEK_SET);


	
	  for(int i=0; i < byteCount * 8; i++)
	  {
	    currentBit = (byte >> (7-bit)) & 1;
	    
	    bit++;
	    if(currentBit == 0)
	    {
	      temp = temp->left;
	    }
	    else 
	    {
	      temp = temp->right;
	    }
	    
	    if(bit==8){
	      byte = fgetc(file);
	      bit = 0;
	    }
	    if(temp->right==NULL && temp->left == NULL){
	      	fwrite(&temp->c, sizeof(char), 1, newFile);
	      	temp = treeRoot;
	    }
	    

	  }

	  
	}
