#ifndef __USER_RTC_CONFIG_H__
#define __USER_RTC_CONFIG_H__

extern void RTC_CLKSourceConfig(uint8_t ClkSrc, uint8_t FirstLastCfg, uint8_t RstBKP);
extern ErrorStatus RTC_TimeRegulate(void);
extern ErrorStatus RTC_DateRegulate(void);
extern ErrorStatus RTC_AlarmRegulate(uint32_t RTC_Alarm);
extern void RTC_DateAndTimeDefaultVale(void);
extern void RTC_PrescalerConfig(void);
extern void EXTI_PA11_Configuration(void);
extern void RTC_TimeShow(void);
extern void RTC_DateShow(void);
extern void RTC_AlarmShow(uint8_t AlarmX);
extern void EXTI18_RTCAlarm_Configuration(FunctionalState Cmd);

#endif/*__USER_RTC_CONFIG_H__*/
