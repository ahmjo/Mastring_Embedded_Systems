#ifndef UTILS_H_
#define UTILS_H_

#define Set_Bit(reg,bit)  (reg |= (1<<bit))
#define CLR_Bit(reg,bit)  (reg &= ~(1<<bit))
#define Read_Bit(reg,bit)   ((reg>>bit)&1)
#define Toggle_Bit(reg,bit)    reg^=(1<<bit)


#endif /* UTILS_H_ */