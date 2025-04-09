#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include "series.h"

typedef enum {
    IEC63StdSeriesFlagE3      = (1u << IEC63StandartSeriesE3)
    , IEC63StdSeriesFlagE6    = (1u << IEC63StandartSeriesE6)
    , IEC63StdSeriesFlagE12   = (1u << IEC63StandartSeriesE12)
    , IEC63StdSeriesFlagE24   = (1u << IEC63StandartSeriesE24)
    , IEC63StdSeriesFlagE48   = (1u << IEC63StandartSeriesE48)
    , IEC63StdSeriesFlagE96   = (1u << IEC63StandartSeriesE96)
    , IEC63StdSeriesFlagE192  = (1u << IEC63StandartSeriesE192)
} IEC63StdSeriesFlag;
#define IEC63_STD_CORRECT_SERIES(s)    (((s) >= IEC63StandartSeriesNone) && ((s) <= IEC63StandartSeriesE192))

typedef struct {
    uint16_t mantissaTimesHundred;
    uint8_t seriesFlags;
} IEC63Std3SigFigsMantissaItem;

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define ELEMENT_SIZE(arr) (sizeof((arr)[0]))

static const IEC63Std3SigFigsMantissaItem mantissaLookup[] = {
    { 0, ~0u }
    , { 100, ( IEC63StdSeriesFlagE3 | IEC63StdSeriesFlagE6 | IEC63StdSeriesFlagE12 | IEC63StdSeriesFlagE24 | IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 101, ( IEC63StdSeriesFlagE192 ) }
    , { 102, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 104, ( IEC63StdSeriesFlagE192 ) }
    , { 105, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 106, ( IEC63StdSeriesFlagE192 ) }
    , { 107, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 109, ( IEC63StdSeriesFlagE192 ) }
    , { 110, ( IEC63StdSeriesFlagE24 | IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 111, ( IEC63StdSeriesFlagE192 ) }
    , { 113, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 114, ( IEC63StdSeriesFlagE192 ) }
    , { 115, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 117, ( IEC63StdSeriesFlagE192 ) }
    , { 118, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 120, ( IEC63StdSeriesFlagE12 | IEC63StdSeriesFlagE24 | IEC63StdSeriesFlagE192 ) }
    , { 121, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 123, ( IEC63StdSeriesFlagE192 ) }
    , { 124, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 126, ( IEC63StdSeriesFlagE192 ) }
    , { 127, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 129, ( IEC63StdSeriesFlagE192 ) }
    , { 130, ( IEC63StdSeriesFlagE24 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 132, ( IEC63StdSeriesFlagE192 ) }
    , { 133, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 135, ( IEC63StdSeriesFlagE192 ) }
    , { 137, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 138, ( IEC63StdSeriesFlagE192 ) }
    , { 140, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 142, ( IEC63StdSeriesFlagE192 ) }
    , { 143, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 145, ( IEC63StdSeriesFlagE192 ) }
    , { 147, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 149, ( IEC63StdSeriesFlagE192 ) }
    , { 150, ( IEC63StdSeriesFlagE6 | IEC63StdSeriesFlagE12 | IEC63StdSeriesFlagE24 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 152, ( IEC63StdSeriesFlagE192 ) }
    , { 154, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 156, ( IEC63StdSeriesFlagE192 ) }
    , { 158, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 160, ( IEC63StdSeriesFlagE24 | IEC63StdSeriesFlagE192 ) }
    , { 162, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 164, ( IEC63StdSeriesFlagE192 ) }
    , { 165, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 167, ( IEC63StdSeriesFlagE192 ) }
    , { 169, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 172, ( IEC63StdSeriesFlagE192 ) }
    , { 174, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 176, ( IEC63StdSeriesFlagE192 ) }
    , { 178, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 180, ( IEC63StdSeriesFlagE12 | IEC63StdSeriesFlagE24 | IEC63StdSeriesFlagE192 ) }
    , { 182, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 184, ( IEC63StdSeriesFlagE192 ) }
    , { 187, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 189, ( IEC63StdSeriesFlagE192 ) }
    , { 191, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 193, ( IEC63StdSeriesFlagE192 ) }
    , { 196, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 198, ( IEC63StdSeriesFlagE192 ) }
    , { 200, ( IEC63StdSeriesFlagE24 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 203, ( IEC63StdSeriesFlagE192 ) }
    , { 205, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 208, ( IEC63StdSeriesFlagE192 ) }
    , { 210, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 213, ( IEC63StdSeriesFlagE192 ) }
    , { 215, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 218, ( IEC63StdSeriesFlagE192 ) }
    , { 220, ( IEC63StdSeriesFlagE3 | IEC63StdSeriesFlagE6 | IEC63StdSeriesFlagE12 | IEC63StdSeriesFlagE24 ) }
    , { 221, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 223, ( IEC63StdSeriesFlagE192 ) }
    , { 226, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 229, ( IEC63StdSeriesFlagE192 ) }
    , { 232, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 234, ( IEC63StdSeriesFlagE192 ) }
    , { 237, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 240, ( IEC63StdSeriesFlagE24 | IEC63StdSeriesFlagE192 ) }
    , { 243, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 246, ( IEC63StdSeriesFlagE192 ) }
    , { 249, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 252, ( IEC63StdSeriesFlagE192 ) }
    , { 255, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 258, ( IEC63StdSeriesFlagE192 ) }
    , { 261, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 264, ( IEC63StdSeriesFlagE192 ) }
    , { 267, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 270, ( IEC63StdSeriesFlagE12 | IEC63StdSeriesFlagE24 ) }
    , { 271, ( IEC63StdSeriesFlagE192 ) }
    , { 274, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 277, ( IEC63StdSeriesFlagE192 ) }
    , { 280, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 284, ( IEC63StdSeriesFlagE192 ) }
    , { 287, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 291, ( IEC63StdSeriesFlagE192 ) }
    , { 294, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 298, ( IEC63StdSeriesFlagE192 ) }
    , { 300, ( IEC63StdSeriesFlagE24 ) }
    , { 301, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 305, ( IEC63StdSeriesFlagE192 ) }
    , { 309, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 312, ( IEC63StdSeriesFlagE192 ) }
    , { 316, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 320, ( IEC63StdSeriesFlagE192 ) }
    , { 324, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 328, ( IEC63StdSeriesFlagE192 ) }
    , { 330, ( IEC63StdSeriesFlagE6 | IEC63StdSeriesFlagE12 | IEC63StdSeriesFlagE24 ) }
    , { 332, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 336, ( IEC63StdSeriesFlagE192 ) }
    , { 340, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 344, ( IEC63StdSeriesFlagE192 ) }
    , { 348, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 352, ( IEC63StdSeriesFlagE192 ) }
    , { 357, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 360, ( IEC63StdSeriesFlagE24 ) }
    , { 361, ( IEC63StdSeriesFlagE192 ) }
    , { 365, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 370, ( IEC63StdSeriesFlagE192 ) }
    , { 374, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 379, ( IEC63StdSeriesFlagE192 ) }
    , { 383, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 388, ( IEC63StdSeriesFlagE192 ) }
    , { 390, ( IEC63StdSeriesFlagE12 | IEC63StdSeriesFlagE24 ) }
    , { 392, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 397, ( IEC63StdSeriesFlagE192 ) }
    , { 402, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 407, ( IEC63StdSeriesFlagE192 ) }
    , { 412, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 417, ( IEC63StdSeriesFlagE192 ) }
    , { 422, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 427, ( IEC63StdSeriesFlagE192 ) }
    , { 430, ( IEC63StdSeriesFlagE24 ) }
    , { 432, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 437, ( IEC63StdSeriesFlagE192 ) }
    , { 442, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 448, ( IEC63StdSeriesFlagE192 ) }
    , { 453, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 459, ( IEC63StdSeriesFlagE192 ) }
    , { 464, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 470, ( IEC63StdSeriesFlagE3 | IEC63StdSeriesFlagE6 | IEC63StdSeriesFlagE12 | IEC63StdSeriesFlagE24 | IEC63StdSeriesFlagE192 ) }
    , { 475, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 481, ( IEC63StdSeriesFlagE192 ) }
    , { 487, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 493, ( IEC63StdSeriesFlagE192 ) }
    , { 499, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 505, ( IEC63StdSeriesFlagE192 ) }
    , { 510, ( IEC63StdSeriesFlagE24 ) }
    , { 511, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 517, ( IEC63StdSeriesFlagE192 ) }
    , { 523, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 530, ( IEC63StdSeriesFlagE192 ) }
    , { 536, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 542, ( IEC63StdSeriesFlagE192 ) }
    , { 549, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 556, ( IEC63StdSeriesFlagE192 ) }
    , { 560, ( IEC63StdSeriesFlagE12 | IEC63StdSeriesFlagE24 ) }
    , { 562, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 569, ( IEC63StdSeriesFlagE192 ) }
    , { 576, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 583, ( IEC63StdSeriesFlagE192 ) }
    , { 590, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 597, ( IEC63StdSeriesFlagE192 ) }
    , { 604, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 612, ( IEC63StdSeriesFlagE192 ) }
    , { 619, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 620, ( IEC63StdSeriesFlagE24 ) }
    , { 626, ( IEC63StdSeriesFlagE192 ) }
    , { 634, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 642, ( IEC63StdSeriesFlagE192 ) }
    , { 649, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 657, ( IEC63StdSeriesFlagE192 ) }
    , { 665, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 673, ( IEC63StdSeriesFlagE192 ) }
    , { 680, ( IEC63StdSeriesFlagE6 | IEC63StdSeriesFlagE12 | IEC63StdSeriesFlagE24 ) }
    , { 681, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 690, ( IEC63StdSeriesFlagE192 ) }
    , { 698, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 706, ( IEC63StdSeriesFlagE192 ) }
    , { 715, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 723, ( IEC63StdSeriesFlagE192 ) }
    , { 732, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 741, ( IEC63StdSeriesFlagE192 ) }
    , { 750, ( IEC63StdSeriesFlagE24 | IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 759, ( IEC63StdSeriesFlagE192 ) }
    , { 768, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 777, ( IEC63StdSeriesFlagE192 ) }
    , { 787, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 796, ( IEC63StdSeriesFlagE192 ) }
    , { 806, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 816, ( IEC63StdSeriesFlagE192 ) }
    , { 820, ( IEC63StdSeriesFlagE12 | IEC63StdSeriesFlagE24 ) }
    , { 825, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 835, ( IEC63StdSeriesFlagE192 ) }
    , { 845, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 856, ( IEC63StdSeriesFlagE192 ) }
    , { 866, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 876, ( IEC63StdSeriesFlagE192 ) }
    , { 887, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 898, ( IEC63StdSeriesFlagE192 ) }
    , { 909, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 910, ( IEC63StdSeriesFlagE24 ) }
    , { 920, ( IEC63StdSeriesFlagE192 ) }
    , { 931, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 942, ( IEC63StdSeriesFlagE192 ) }
    , { 953, ( IEC63StdSeriesFlagE48 | IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 965, ( IEC63StdSeriesFlagE192 ) }
    , { 976, ( IEC63StdSeriesFlagE96 | IEC63StdSeriesFlagE192 ) }
    , { 988, ( IEC63StdSeriesFlagE192 ) }
    , { 1000, 0 }   // Data border. Allways here!
};

static double frexp10(double arg, int * exp)
{
    *exp = (arg == 0) ? 0 : 1 + (int)floor(log10(fabs(arg) ) );
    return arg * pow(10 , -(*exp));
}

static void convertFromDouble(double from, IEC63Standart3SigFigsValue * to)
{
    int order;
    if (from < 0) from = 0.0;
    double mantissa = frexp10(from, &order);
    order--;
    to->mantissaTimesHundred = (uint16_t)round( mantissa * 1000 );
    to->order = (SIStandartValueOrder)order;
    to->series = IEC63StandartSeriesNone;
    if (to->mantissaTimesHundred == 1000) {
        to->mantissaTimesHundred = 100;
        to->order++;
    }
}

static int mantissaLookupRangeComparator(const void * key, const void * datum)
{
    const IEC63Standart3SigFigsValue * k = (const IEC63Standart3SigFigsValue *) key;
    const IEC63Std3SigFigsMantissaItem * d = (const IEC63Std3SigFigsMantissaItem *) datum;
    const IEC63Std3SigFigsMantissaItem * next = (d->seriesFlags) ? (d + 1) : d;

    if (k->mantissaTimesHundred < d->mantissaTimesHundred) {
        return -1;
    } else if (k->mantissaTimesHundred >= next->mantissaTimesHundred) {
        return 1;
    }
    return 0;
}

static /*carry flag*/ bool findBoundsForSuitableSeries(const IEC63Standart3SigFigsValue * key, const IEC63Std3SigFigsMantissaItem * bounds[2], IEC63StandartSeries series)
{
    assert(key->mantissaTimesHundred < 1000);
    bounds[0] = bsearch(key, mantissaLookup, ARRAY_SIZE(mantissaLookup), ELEMENT_SIZE(mantissaLookup), mantissaLookupRangeComparator);
    assert(bounds[0]);
    assert(bounds[0]->mantissaTimesHundred != 1000);
    assert(IEC63_STD_CORRECT_SERIES(series));
    assert(1u << series);

    bounds[1] = bounds[0] + 1;  // mantissaTimesHundred can be equal 1000 for second value

    while( !(bounds[0]->seriesFlags & (1u << series) ) ) --bounds[0];    // rewind bounds to suitable series
    while( bounds[1]->seriesFlags && !(bounds[1]->seriesFlags & (1u << series) ) ) ++bounds[1];

    if (!(bounds[1]->seriesFlags)) {
        const IEC63Std3SigFigsMantissaItem * ptrToHundred = &mantissaLookup[0];
        while(ptrToHundred->mantissaTimesHundred < 100) ptrToHundred++;
        bounds[1] = ptrToHundred;
        return true;
    }
    return false;
}

void standartValueDecompose(IEC63Standart3SigFigsValue * obj, double value, IEC63StandartSeries series, IEC63StandartRoundingPolicy rounding)
{
    IEC63Standart3SigFigsValue notNormalized;
    const IEC63Std3SigFigsMantissaItem * bounds[2] = {NULL, NULL};
    bool carry;
    if (series == IEC63StandartSeriesNone) {
        series = IEC63StandartSeriesE24;
    }
    convertFromDouble(value, &notNormalized);
    carry = findBoundsForSuitableSeries(&notNormalized, bounds, series);
    obj->order = notNormalized.order;
    obj->series = series;

    switch (rounding) {
    case IEC63StandartRoundingPolicyCeil:
        if(bounds[0]->mantissaTimesHundred == notNormalized.mantissaTimesHundred) {
            obj->mantissaTimesHundred = bounds[0]->mantissaTimesHundred;
        } else {
            if (carry) obj->order++;
            obj->mantissaTimesHundred = bounds[1]->mantissaTimesHundred;
        }
        break;
    case IEC63StandartRoundingPolicyFloor:
        obj->mantissaTimesHundred = bounds[0]->mantissaTimesHundred;
        break;
    case IEC63StandartRoundingPolicyNearest:
    default:
        do {
            int delta_low = notNormalized.mantissaTimesHundred - bounds[0]->mantissaTimesHundred;
            int delta_high = bounds[1]->mantissaTimesHundred - notNormalized.mantissaTimesHundred;
            if(delta_low < delta_high) {
                obj->mantissaTimesHundred = bounds[0]->mantissaTimesHundred;
            } else {
                if (carry) obj->order++;
                obj->mantissaTimesHundred = bounds[1]->mantissaTimesHundred;
            }
        } while(0);
        break;
    }
}

double standartValueCompose(const IEC63Standart3SigFigsValue * obj)
{
    return (double)obj->mantissaTimesHundred * pow(10, obj->order - 2);
}

void standartValueIncrement(IEC63Standart3SigFigsValue * obj)
{
    if (obj->mantissaTimesHundred == 0) return;

    IEC63StandartSeries targetSeries = obj->series;
    bool carry;
    if (targetSeries == IEC63StandartSeriesNone) {
        targetSeries = IEC63StandartSeriesE24;
    }
    const IEC63Std3SigFigsMantissaItem * bounds[2] = {NULL, NULL};
    carry = findBoundsForSuitableSeries(obj, bounds, targetSeries);
    if (carry) obj->order++;
    obj->mantissaTimesHundred = bounds[1]->mantissaTimesHundred;
}

void standartValueDecrement(IEC63Standart3SigFigsValue * obj)
{
    if (obj->mantissaTimesHundred == 0) return;

    IEC63StandartSeries targetSeries = obj->series;
    if (targetSeries == IEC63StandartSeriesNone) {
        targetSeries = IEC63StandartSeriesE24;
    }
    if (obj->mantissaTimesHundred == 100) {
        obj->mantissaTimesHundred = 999;
        obj->order--;
    } else {
        obj->mantissaTimesHundred--;
    }
    const IEC63Std3SigFigsMantissaItem * bounds[2] = {NULL, NULL};
    findBoundsForSuitableSeries(obj, bounds, targetSeries);
    obj->mantissaTimesHundred = bounds[0]->mantissaTimesHundred;
}

bool standartValueEq(const IEC63Standart3SigFigsValue * left, const IEC63Standart3SigFigsValue * right)
{
    return ( (left->order == right->order) && (left->mantissaTimesHundred == right->mantissaTimesHundred) );
}

bool standartValueGt(const IEC63Standart3SigFigsValue * left, const IEC63Standart3SigFigsValue * right)
{
    return ( (left->order >= right->order) && (left->mantissaTimesHundred > right->mantissaTimesHundred) );
}

void standartValueSetSeries(IEC63Standart3SigFigsValue * obj, IEC63StandartSeries series)
{
    if( obj->series == series ) return;
    const IEC63Std3SigFigsMantissaItem * bounds[2] = {NULL, NULL};
    bool carry;
    carry = findBoundsForSuitableSeries(obj, bounds, series);

    int delta_low = obj->mantissaTimesHundred - bounds[0]->mantissaTimesHundred;
    int delta_high = bounds[1]->mantissaTimesHundred - obj->mantissaTimesHundred;
    if(delta_low < delta_high) {
        obj->mantissaTimesHundred = bounds[0]->mantissaTimesHundred;
    } else {
        if (carry) obj->order++;
        obj->mantissaTimesHundred = bounds[1]->mantissaTimesHundred;
    }
}
