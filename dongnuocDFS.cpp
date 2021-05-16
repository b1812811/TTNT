#include <stdio.h>
#include <stdlib.h>

#include <stack>

using namespace std;

#define tankcapacity_X 9
#define tankcapacity_Y 4
#define tankempty 0
#define goal 6
#define Maxlength 100

// Hang chuoi de in ra ten cac hanh dong
const char* action[] = {"First State",      
                                            "pour Walter Full X",
                                            "pour Walter Full Y",
                                            "pour Walter Empty X",
                                            "pour Walter Empty Y",
                                            "pour Walter X to Y",
                                            "pour Walter Y to X"};

// Khai bao cau truc trang thai
typedef struct {
	int x; // Luong nuoc trong binh x
	int y; // Luong nuoc trong binh y
} State;

// Khoi tao trang thai binh X = 0 va binh Y = 0
void makeNullState(State *state) {
    state->x = 0;
    state->y = 0;
}

// Ham in trang thai
void print_State(State state) {
	printf("\n X: %d --- Y: %d", state.x, state.y);
}

// Ham kiem tra trang thai co phai muc tieu
int goalcheck(State state) {
    return (state.x==goal || state.y==goal);
}

int min(int x, int y){
    return x<y ? x : y;
}

int max(int x, int y){
    return x>y ? x : y;
}

// Ham lam day binh nuoc X
int pourWalterFullX(State cur_state, State *result) {
    if(cur_state.x < tankcapacity_X) {
        result->x = tankcapacity_X;
        result->y = cur_state.y;
        return 1;
    }
    
    return 0;
}

// Ham lam day binh nuoc Y
int pourWalterFullY(State cur_state, State *result) {
    if(cur_state.y < tankcapacity_Y) {
        result->x = cur_state.x;
        result->y = tankcapacity_Y;
        return 1;
    }
    return 0;
}

// Ham lam rong binh nuoc X
int pourWalterEmptyX(State cur_state, State *result){
    if(cur_state.x>0){
        result->x = tankempty;
        result->y = cur_state.y;
        return 1;
    }
    return 0;
}

// Ham lam rong binh nuoc Y
int pourWalterEmptyY(State cur_state, State *result){
    if(cur_state.y>0){
        result->y = tankempty;
        result->x = cur_state.x;
        return 1;
    }
    return 0;
}

// Ham chuyen nuoc tu X sang Y
int pourWalterXY(State cur_state, State *result){
    if(cur_state.x>0 && cur_state.y<tankcapacity_Y){
        result->x = max(cur_state.x - (tankcapacity_Y - cur_state.y), tankempty);
        result->y = min(cur_state.x+cur_state.y, tankcapacity_Y);
        return 1;
    }
    return 0;
}

// Ham chuyen nuoc tu Y sang X
int pourWalterYX(State cur_state, State *result){
    if(cur_state.y>0 && cur_state.x<tankcapacity_X){
        result->y = max(cur_state.y - (tankcapacity_X - cur_state.x), tankempty);
        result->x = min(cur_state.y + cur_state.x, tankcapacity_X);
        return 1;
    }
    return 0;
}

// Goi cac phep toan trang thai
int call_operator(State cur_state, State *result, int option){
    switch(option){
        case 1: return pourWalterFullX(cur_state, result);
        case 2: return pourWalterFullY(cur_state, result);
        case 3: return pourWalterEmptyX(cur_state, result);
        case 4: return pourWalterEmptyY(cur_state, result);
        case 5: return pourWalterXY(cur_state, result);
        case 6: return pourWalterYX(cur_state, result);
        default: printf("ERROR calls operators");
            return 0;
    }
}

// So sanh hai trang thai co bang nhau khong
int compareStates(State state1, State state2){
    return (state1.x == state2.x && state1.y == state2.y);
}

// Khai bao cau truc nut (dinh) de xay dung cay tim kiem
// State : dung de luu trang thai luong nuoc  trong mot binh x va binh Y
// Parent : dung de luu nut cha cua mot nut moi duoc trien khai
// no_function : luu stt phep toan da thuc hien tuong ung. Bai toan dong nuoc chung ta co 6 phep toan.

typedef struct Node{
    State state;
    struct Node* Parent;
    int no_function;
} Node;

int find_State(State state, stack<Node*> openStack){
    while(!openStack.empty()){
        if(compareStates(openStack.top()->state, state))
            return 1;
        openStack.pop();
    }
    return 0;
}

// Thuat toan duyet theo chieu rong
Node* DFS_Algorithm(State state){
    
    // Khai bao hai ngan xep Open va Close
    stack<Node*> Open_DFS;
    stack<Node*> Close_DFS;
    
    //Tao nut trang thai cha
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->Parent = NULL;
    root->no_function = 0;
    Open_DFS.push(root);
    while(!Open_DFS.empty()){
		// Lay mot dinh trong ngan xep
        Node* node = Open_DFS.top(); 
        Open_DFS.pop();
        Close_DFS.push(node);
        
        // Kiem tra xem dinh lay ra co phai la trang thai muc tieu
        if(goalcheck(node->state))
            return node;
        int option;
        // Goi cac phep toan tren trang thai
        for(option=1; option<=6; option++) {
            State newstate;
            makeNullState(&newstate);
            if(call_operator(node->state, &newstate, option)) {
                // Neu trang thai moi sinh ra da ton tai thi bo qua
                if(find_State(newstate, Open_DFS) || find_State(newstate, Close_DFS))
                    continue;
                // Neu trang thai moi chua ton tai thi them vao ngan xep
                Node *newNode = (Node*)malloc(sizeof(Node));
                newNode->state = newstate;
                newNode->Parent = node;
                newNode->no_function = option;
                Open_DFS.push(newNode);
            }
        }
        
    }
    return NULL;
}

// In ket qua chuyen nuoc de dat den trang thai muc tieu
void print_WaysToGetGoal(Node* node) {
    stack<Node*> stackPrint;
    
    // Duyet nguoc ve nut Parent
    while(node->Parent != NULL){
        stackPrint.push(node);
        node = node->Parent;
    }
    stackPrint.push(node);
    
    // In ra thu tu hanh dong truyen nuoc
    int no_action = 0;
    while(!stackPrint.empty()){
        printf("\nAction %d: %s", no_action, action[stackPrint.top()->no_function]);
        print_State(stackPrint.top()->state);
        stackPrint.pop();
        no_action++;
    }
    
}

int main() {
	State cur_state = {0, 0};
	
	printf("Trang thai bat dau");
	print_State(cur_state);
	
	Node* p = DFS_Algorithm(cur_state);
	print_WaysToGetGoal(p);

}