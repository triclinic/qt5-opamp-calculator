#ifndef SERIES_H
#define SERIES_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __cplusplus
#ifndef CPP_STYLE_BOOL
#define CPP_STYLE_BOOL
typedef _Bool bool;
#define false 0
#define true 1
#endif
#endif

typedef int IEC63StandartValueOrder;
typedef uint16_t IEC63StandartValueMantissa;

/* Standart IEC63 prefixes */
typedef enum {
    IEC63StandartSeriesNone
    , IEC63StandartSeriesE3
    , IEC63StandartSeriesE6
    , IEC63StandartSeriesE12
    , IEC63StandartSeriesE24
    , IEC63StandartSeriesE48
    , IEC63StandartSeriesE96
    , IEC63StandartSeriesE192
} IEC63StandartSeries;

typedef enum {
    IEC63StandartRoundingPolicyNearest
    , IEC63StandartRoundingPolicyCeil
    , IEC63StandartRoundingPolicyFloor
} IEC63StandartRoundingPolicy;

typedef struct {
    IEC63StandartValueMantissa mantissaTimesHundred;
    IEC63StandartValueOrder order;
    IEC63StandartSeries series;
} IEC63Standart3SigFigsValue;

void iec63StandartValueInit(IEC63Standart3SigFigsValue *, double value, IEC63StandartSeries series, IEC63StandartRoundingPolicy);
void iec63StandartValueSetSeries(IEC63Standart3SigFigsValue *, IEC63StandartSeries series);
double iec63StandartToDouble(const IEC63Standart3SigFigsValue *);
void iec63StandartValueIncrement(IEC63Standart3SigFigsValue *);
void iec63StandartValueDecrement(IEC63Standart3SigFigsValue *);
bool iec63StandartValueEq(const IEC63Standart3SigFigsValue *, const IEC63Standart3SigFigsValue *);
bool iec63StandartValueGt(const IEC63Standart3SigFigsValue *, const IEC63Standart3SigFigsValue *);

#ifdef __cplusplus
}
#endif

#endif
