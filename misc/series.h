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

/* Standart SI prefixes */

typedef int SIStandartValueOrder;

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
    uint16_t mantissaTimesHundred;
    SIStandartValueOrder order;
    IEC63StandartSeries series;
} IEC63Standart3SigFigsValue;

void standartValueDecompose(IEC63Standart3SigFigsValue *, double value, IEC63StandartSeries series, IEC63StandartRoundingPolicy);
void standartValueSetSeries(IEC63Standart3SigFigsValue *, IEC63StandartSeries series);
double standartValueCompose(const IEC63Standart3SigFigsValue *);
void standartValueIncrement(IEC63Standart3SigFigsValue *);
void standartValueDecrement(IEC63Standart3SigFigsValue *);
bool standartValueEq(const IEC63Standart3SigFigsValue *, const IEC63Standart3SigFigsValue *);
bool standartValueGt(const IEC63Standart3SigFigsValue *, const IEC63Standart3SigFigsValue *);

#ifdef __cplusplus
}
#endif

#endif
