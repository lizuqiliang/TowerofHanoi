#ifndef stack_h
#define stack_h

struct NODE{
	float n;
	NODE* next;
};

class stack{
	public:
		stack();
		~stack();
		void push(float f);
		void display();
		int getlength();
		float show();
		float pop();
	private:
		int length;
		NODE* head;
};

#endif

