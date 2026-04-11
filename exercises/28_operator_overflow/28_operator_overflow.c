#include <stdio.h>
#include <limits.h>

#define CHECK_OVERFLOW(carry) \
    carry ? "Overflow" : "Not Overflow"

// ✅ flam 知识点：无符号加法看 CF（Carry Flag），CF=1 代表发生了 32 位进位（回绕）
int check_add_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char carry;
    __asm__ volatile(
        // ✅ flam：addl 做 32 位无符号加法，结果写回 a，对应的 EFLAGS 会被更新
        "addl %[b], %[a]\n\t"
        // ✅ flam：setc 读取 CF，若 CF=1 则 carry=1，否则 carry=0
        "setc %[carry]\n\t"
        // ✅ flam："+r"(a) 表示 a 既是输入也是输出；"=qm"(carry) 允许寄存器或内存 8 位写回
        : [a] "+r"(a), [carry] "=qm"(carry)
        : [b] "r"(b)
        // ✅ flam："cc" 告诉编译器条件码寄存器（EFLAGS）被修改
        : "cc"
    );
    return carry;
}

// ✅ flam 知识点：无符号减法看 CF，CF=1 代表发生借位（a < b）
int check_sub_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char carry;
    __asm__ volatile(
        // ✅ flam：subl 执行 a-b，并更新 EFLAGS
        "subl %[b], %[a]\n\t"
        // ✅ flam：setc 读取 CF；这里 CF=1 就是无符号下溢（借位）
        "setc %[carry]\n\t"
        : [a] "+r"(a), [carry] "=qm"(carry)
        : [b] "r"(b)
        : "cc"
    );
    return carry;
}

// ✅ flam 知识点：无符号乘法使用 mull，64 位结果放在 EDX:EAX
int check_mul_overflow_asm(unsigned int a, unsigned int b) {
    unsigned int high_bits;
    unsigned char overflow;
    __asm__ volatile(
        // ✅ flam："+a"(a) 强制 a 进入 EAX，mull 后低 32 位在 EAX，高 32 位在 EDX
        "mull %[b]\n\t"
        // ✅ flam：mull 若高位非 0，会把 CF/OF 置 1；setc 把 CF 提取到 overflow
        "setc %[overflow]\n\t"
        // ✅ flam："=d"(high_bits) 读取 EDX 高 32 位，双保险判断更直观
        : "+a"(a), "=d"(high_bits), [overflow] "=qm"(overflow)
        : [b] "r"(b)
        : "cc"
    );
    // ✅ flam：任一条件为真都视为乘法溢出
    return overflow || (high_bits != 0);
}

// ✅ flam 知识点：除法这里按题目语义把“除 0”视作 overflow
int check_div_overflow_asm(unsigned int a, unsigned int b) {
    unsigned char is_div_zero;
    (void)a;
    __asm__ volatile(
        // ✅ flam：比较 b 和 0，仅检测除零风险，不实际执行 div 指令
        "cmpl $0, %[b]\n\t"
        // ✅ flam：若 b==0，则 ZF=1，sete 写 1；否则写 0
        "sete %[is_div_zero]\n\t"
        : [is_div_zero] "=qm"(is_div_zero)
        : [b] "r"(b)
        : "cc"
    );
    return is_div_zero;
}

int main() {
    printf("(UINT_MAX + 1)Add: %s\n", CHECK_OVERFLOW(check_add_overflow_asm(UINT_MAX, 1)));   // 1
    printf("(1, 0)Add: %s\n", CHECK_OVERFLOW(check_add_overflow_asm(1, 0)));  
    printf("(0, 1)Sub: %s\n", CHECK_OVERFLOW(check_sub_overflow_asm(0, 1)));          // 1
    printf("(2, 1)Sub: %s\n", CHECK_OVERFLOW(check_sub_overflow_asm(2, 1)));
    printf("(UINT_MAX, 2)Mul: %s\n", CHECK_OVERFLOW(check_mul_overflow_asm(UINT_MAX, 2)));   // 1
    printf("(1, 2)Mul: %s\n", CHECK_OVERFLOW(check_mul_overflow_asm(1, 2)));
    printf("(10, 0)Div: %s\n", CHECK_OVERFLOW(check_div_overflow_asm(10, 0)));                         // 1
    printf("(2, 1)Div: %s\n", CHECK_OVERFLOW(check_div_overflow_asm(2, 1)));
    return 0;
}
