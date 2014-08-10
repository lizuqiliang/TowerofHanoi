#ifndef STACK_CPP
#define STACK_CPP

stack::stack(){
	head = NULL;
	length = 0;
}

stack::~stack(){
	NODE* curr = head;
	while( curr != NULL ) {
		NODE* next = curr->next;
		delete curr;
		curr = next;
	}
	head = NULL;
}

void stack::push(float f){
	NODE* temp = new NODE;
	temp->n=f;
	temp->next=head;
	head = temp;
	length ++;
}

float stack::pop(){
	NODE* temp = head->next;
	float num=head->n;
	delete head;
	head = temp;
	length --;
	return num;
}

int stack::getlength(){
     return(length);
}

float stack::show(){
	return head->n;
}

#endif
