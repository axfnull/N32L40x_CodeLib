#ifndef __USER_SYSCLOCK_CONFIG_H__
#define __USER_SYSCLOCK_CONFIG_H__

typedef enum
{
    SYSCLK_PLLSRC_HSI,
    SYSCLK_PLLSRC_HSIDIV2,
    SYSCLK_PLLSRC_HSI_PLLDIV2,
    SYSCLK_PLLSRC_HSIDIV2_PLLDIV2,
    SYSCLK_PLLSRC_HSE,
    SYSCLK_PLLSRC_HSEDIV2,
    SYSCLK_PLLSRC_HSE_PLLDIV2,
    SYSCLK_PLLSRC_HSEDIV2_PLLDIV2,
}SYSCLK_PLL_TYPE;

extern void SetSysClockToPLL(uint32_t freq, SYSCLK_PLL_TYPE src);

#endif/*__USER_SYSCLOCK_CONFIG_H__*/

