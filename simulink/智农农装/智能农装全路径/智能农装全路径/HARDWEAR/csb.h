#ifndef _CSB_H_
#define _CSB_H_

typedef enum{
	CSB_Front,
	CSB_Back,
	CSB_Left,
	CSB_Right
}CSB_Type;

typedef struct{
	float CSB_Count_Front;
	float CSB_Count_Back;
	float CSB_Count_Left;
	float CSB_Count_Right;
}CSB_Count;

typedef struct{
	float CSB_Distance_Front;
	float CSB_Distance_Back;
	float CSB_Distance_Left;
	float CSB_Distance_Right;
}CSB_Distance_Data;

typedef struct{
	int CSB_Allow_Front;
	int CSB_Allow_Back;
	int CSB_Allow_Left;
	int CSB_Allow_Right;
}CSB_Allow_type;
////????????
//CSB_Distance_Data CSB_Disance;
//CSB_Count CSB_OriData;
//CSB_Allow_type CSB_Allow;
////????????
//int CSB_Freshed_Flag;
//int CSB_Require_Flag;
//int CSB_GetData_Flag;

void CSB_Init(void);
void CSB_Allow_Which(uint8_t Allow_order);
void CSB_Enable(void);
void CSB_Disable(void);

void CSB_Fresh_Count(void);
void CSB_Clear_Count(void);
void CSB_Fresh_Distance(void);
int CSB_AllGot(void);

void CSB_Service(void);
void CSB_Show(void);
float CSB_GetDistance(CSB_Type which_type);

#endif
