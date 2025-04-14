#include "series.h"
#include <math.h>
#include <stdlib.h>
#include <assert.h>

typedef enum {
    IEC63StandartSeriesFlagE3      = (1u << IEC63StandartSeriesE3)
    , IEC63StandartSeriesFlagE6    = (1u << IEC63StandartSeriesE6)
    , IEC63StandartSeriesFlagE12   = (1u << IEC63StandartSeriesE12)
    , IEC63StandartSeriesFlagE24   = (1u << IEC63StandartSeriesE24)
    , IEC63StandartSeriesFlagE12_24   = ( IEC63StandartSeriesFlagE12 | IEC63StandartSeriesFlagE24 )
    , IEC63StandartSeriesFlagE6_24   = ( IEC63StandartSeriesFlagE6 | IEC63StandartSeriesFlagE12_24 )
    , IEC63StandartSeriesFlagE3_24   = ( IEC63StandartSeriesFlagE3 | IEC63StandartSeriesFlagE6_24 )

    , IEC63StandartSeriesFlagE48   = (1u << IEC63StandartSeriesE48)
    , IEC63StandartSeriesFlagE96   = (1u << IEC63StandartSeriesE96)
    , IEC63StandartSeriesFlagE192  = (1u << IEC63StandartSeriesE192)
    , IEC63StandartSeriesFlagE96_192   = ( IEC63StandartSeriesFlagE96 | IEC63StandartSeriesFlagE192 )
    , IEC63StandartSeriesFlagE48_192   = ( IEC63StandartSeriesFlagE48 | IEC63StandartSeriesFlagE96_192 )
} IEC63StandartSeriesFlag;
#define IEC63_STD_CORRECT_SERIES(s)    (((s) >= IEC63StandartSeriesNone) && ((s) <= IEC63StandartSeriesE192))

typedef struct {
    IEC63StandartValueMantissa mantissaTimesHundred;
    uint8_t seriesFlags;
} IEC63Std3SigFigsMantissaItem;

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define ELEMENT_SIZE(arr) (sizeof((arr)[0]))

static const IEC63Std3SigFigsMantissaItem mantissaLookup[] = {
    { 0, ( IEC63StandartSeriesFlagE3_24 | IEC63StandartSeriesFlagE48_192 ) }
    , { 100, ( IEC63StandartSeriesFlagE3_24 | IEC63StandartSeriesFlagE48_192 ) }
    , { 101, ( IEC63StandartSeriesFlagE192 ) }
    , { 102, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 104, ( IEC63StandartSeriesFlagE192 ) }
    , { 105, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 106, ( IEC63StandartSeriesFlagE192 ) }
    , { 107, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 109, ( IEC63StandartSeriesFlagE192 ) }
    , { 110, ( IEC63StandartSeriesFlagE24 | IEC63StandartSeriesFlagE48_192 ) }
    , { 111, ( IEC63StandartSeriesFlagE192 ) }
    , { 113, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 114, ( IEC63StandartSeriesFlagE192 ) }
    , { 115, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 117, ( IEC63StandartSeriesFlagE192 ) }
    , { 118, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 120, ( IEC63StandartSeriesFlagE12_24 | IEC63StandartSeriesFlagE192 ) }
    , { 121, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 123, ( IEC63StandartSeriesFlagE192 ) }
    , { 124, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 126, ( IEC63StandartSeriesFlagE192 ) }
    , { 127, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 129, ( IEC63StandartSeriesFlagE192 ) }
    , { 130, ( IEC63StandartSeriesFlagE24 | IEC63StandartSeriesFlagE96_192 ) }
    , { 132, ( IEC63StandartSeriesFlagE192 ) }
    , { 133, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 135, ( IEC63StandartSeriesFlagE192 ) }
    , { 137, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 138, ( IEC63StandartSeriesFlagE192 ) }
    , { 140, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 142, ( IEC63StandartSeriesFlagE192 ) }
    , { 143, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 145, ( IEC63StandartSeriesFlagE192 ) }
    , { 147, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 149, ( IEC63StandartSeriesFlagE192 ) }
    , { 150, ( IEC63StandartSeriesFlagE6_24 | IEC63StandartSeriesFlagE96_192 ) }
    , { 152, ( IEC63StandartSeriesFlagE192 ) }
    , { 154, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 156, ( IEC63StandartSeriesFlagE192 ) }
    , { 158, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 160, ( IEC63StandartSeriesFlagE24 | IEC63StandartSeriesFlagE192 ) }
    , { 162, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 164, ( IEC63StandartSeriesFlagE192 ) }
    , { 165, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 167, ( IEC63StandartSeriesFlagE192 ) }
    , { 169, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 172, ( IEC63StandartSeriesFlagE192 ) }
    , { 174, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 176, ( IEC63StandartSeriesFlagE192 ) }
    , { 178, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 180, ( IEC63StandartSeriesFlagE12_24 | IEC63StandartSeriesFlagE192 ) }
    , { 182, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 184, ( IEC63StandartSeriesFlagE192 ) }
    , { 187, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 189, ( IEC63StandartSeriesFlagE192 ) }
    , { 191, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 193, ( IEC63StandartSeriesFlagE192 ) }
    , { 196, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 198, ( IEC63StandartSeriesFlagE192 ) }
    , { 200, ( IEC63StandartSeriesFlagE24 | IEC63StandartSeriesFlagE96_192 ) }
    , { 203, ( IEC63StandartSeriesFlagE192 ) }
    , { 205, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 208, ( IEC63StandartSeriesFlagE192 ) }
    , { 210, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 213, ( IEC63StandartSeriesFlagE192 ) }
    , { 215, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 218, ( IEC63StandartSeriesFlagE192 ) }
    , { 220, ( IEC63StandartSeriesFlagE3_24 ) }
    , { 221, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 223, ( IEC63StandartSeriesFlagE192 ) }
    , { 226, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 229, ( IEC63StandartSeriesFlagE192 ) }
    , { 232, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 234, ( IEC63StandartSeriesFlagE192 ) }
    , { 237, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 240, ( IEC63StandartSeriesFlagE24 | IEC63StandartSeriesFlagE192 ) }
    , { 243, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 246, ( IEC63StandartSeriesFlagE192 ) }
    , { 249, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 252, ( IEC63StandartSeriesFlagE192 ) }
    , { 255, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 258, ( IEC63StandartSeriesFlagE192 ) }
    , { 261, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 264, ( IEC63StandartSeriesFlagE192 ) }
    , { 267, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 270, ( IEC63StandartSeriesFlagE12_24 ) }
    , { 271, ( IEC63StandartSeriesFlagE192 ) }
    , { 274, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 277, ( IEC63StandartSeriesFlagE192 ) }
    , { 280, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 284, ( IEC63StandartSeriesFlagE192 ) }
    , { 287, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 291, ( IEC63StandartSeriesFlagE192 ) }
    , { 294, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 298, ( IEC63StandartSeriesFlagE192 ) }
    , { 300, ( IEC63StandartSeriesFlagE24 ) }
    , { 301, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 305, ( IEC63StandartSeriesFlagE192 ) }
    , { 309, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 312, ( IEC63StandartSeriesFlagE192 ) }
    , { 316, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 320, ( IEC63StandartSeriesFlagE192 ) }
    , { 324, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 328, ( IEC63StandartSeriesFlagE192 ) }
    , { 330, ( IEC63StandartSeriesFlagE6_24 ) }
    , { 332, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 336, ( IEC63StandartSeriesFlagE192 ) }
    , { 340, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 344, ( IEC63StandartSeriesFlagE192 ) }
    , { 348, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 352, ( IEC63StandartSeriesFlagE192 ) }
    , { 357, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 360, ( IEC63StandartSeriesFlagE24 ) }
    , { 361, ( IEC63StandartSeriesFlagE192 ) }
    , { 365, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 370, ( IEC63StandartSeriesFlagE192 ) }
    , { 374, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 379, ( IEC63StandartSeriesFlagE192 ) }
    , { 383, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 388, ( IEC63StandartSeriesFlagE192 ) }
    , { 390, ( IEC63StandartSeriesFlagE12_24 ) }
    , { 392, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 397, ( IEC63StandartSeriesFlagE192 ) }
    , { 402, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 407, ( IEC63StandartSeriesFlagE192 ) }
    , { 412, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 417, ( IEC63StandartSeriesFlagE192 ) }
    , { 422, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 427, ( IEC63StandartSeriesFlagE192 ) }
    , { 430, ( IEC63StandartSeriesFlagE24 ) }
    , { 432, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 437, ( IEC63StandartSeriesFlagE192 ) }
    , { 442, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 448, ( IEC63StandartSeriesFlagE192 ) }
    , { 453, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 459, ( IEC63StandartSeriesFlagE192 ) }
    , { 464, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 470, ( IEC63StandartSeriesFlagE3_24 | IEC63StandartSeriesFlagE192 ) }
    , { 475, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 481, ( IEC63StandartSeriesFlagE192 ) }
    , { 487, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 493, ( IEC63StandartSeriesFlagE192 ) }
    , { 499, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 505, ( IEC63StandartSeriesFlagE192 ) }
    , { 510, ( IEC63StandartSeriesFlagE24 ) }
    , { 511, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 517, ( IEC63StandartSeriesFlagE192 ) }
    , { 523, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 530, ( IEC63StandartSeriesFlagE192 ) }
    , { 536, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 542, ( IEC63StandartSeriesFlagE192 ) }
    , { 549, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 556, ( IEC63StandartSeriesFlagE192 ) }
    , { 560, ( IEC63StandartSeriesFlagE12_24 ) }
    , { 562, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 569, ( IEC63StandartSeriesFlagE192 ) }
    , { 576, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 583, ( IEC63StandartSeriesFlagE192 ) }
    , { 590, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 597, ( IEC63StandartSeriesFlagE192 ) }
    , { 604, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 612, ( IEC63StandartSeriesFlagE192 ) }
    , { 619, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 620, ( IEC63StandartSeriesFlagE24 ) }
    , { 626, ( IEC63StandartSeriesFlagE192 ) }
    , { 634, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 642, ( IEC63StandartSeriesFlagE192 ) }
    , { 649, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 657, ( IEC63StandartSeriesFlagE192 ) }
    , { 665, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 673, ( IEC63StandartSeriesFlagE192 ) }
    , { 680, ( IEC63StandartSeriesFlagE6_24 ) }
    , { 681, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 690, ( IEC63StandartSeriesFlagE192 ) }
    , { 698, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 706, ( IEC63StandartSeriesFlagE192 ) }
    , { 715, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 723, ( IEC63StandartSeriesFlagE192 ) }
    , { 732, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 741, ( IEC63StandartSeriesFlagE192 ) }
    , { 750, ( IEC63StandartSeriesFlagE24 | IEC63StandartSeriesFlagE48_192 ) }
    , { 759, ( IEC63StandartSeriesFlagE192 ) }
    , { 768, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 777, ( IEC63StandartSeriesFlagE192 ) }
    , { 787, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 796, ( IEC63StandartSeriesFlagE192 ) }
    , { 806, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 816, ( IEC63StandartSeriesFlagE192 ) }
    , { 820, ( IEC63StandartSeriesFlagE12_24 ) }
    , { 825, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 835, ( IEC63StandartSeriesFlagE192 ) }
    , { 845, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 856, ( IEC63StandartSeriesFlagE192 ) }
    , { 866, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 876, ( IEC63StandartSeriesFlagE192 ) }
    , { 887, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 898, ( IEC63StandartSeriesFlagE192 ) }
    , { 909, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 910, ( IEC63StandartSeriesFlagE24 ) }
    , { 920, ( IEC63StandartSeriesFlagE192 ) }
    , { 931, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 942, ( IEC63StandartSeriesFlagE192 ) }
    , { 953, ( IEC63StandartSeriesFlagE48_192 ) }
    , { 965, ( IEC63StandartSeriesFlagE192 ) }
    , { 976, ( IEC63StandartSeriesFlagE96_192 ) }
    , { 988, ( IEC63StandartSeriesFlagE192 ) }
    , { 1000, 0 }   // Data border. Allways here!
};

static double frexp10(double arg, int * exp)
{
    *exp = (arg == 0) ? 0 : 1 + (int)floor(log10(fabs(arg) ) );
    return arg * pow(10 , -(*exp));
}

static IEC63StandartValueMantissa convertFromDouble(double from, IEC63StandartValueOrder * exp)
{
    IEC63StandartValueMantissa result;
    int order;
    if (from < 1e-30) {
        from = 1e-30;
    } else if (from > 1e30) {
        from = 1e30;
    }
    double mantissa = frexp10(from, &order);
    order--;
    result = (IEC63StandartValueMantissa) round( mantissa * 100 * 10 );
    *exp = (IEC63StandartValueOrder)order;
    if (result == 1000) {
        result = 100;
        (*exp)++;
    }
    return result;
}

static int mantissaLookupRangeComparator(const void * key, const void * datum)
{
    const IEC63StandartValueMantissa target = *(const IEC63StandartValueMantissa *) key;
    const IEC63Std3SigFigsMantissaItem * d = (const IEC63Std3SigFigsMantissaItem *) datum;
    const IEC63Std3SigFigsMantissaItem * next = (d->seriesFlags) ? (d + 1) : d;

    if (target < d->mantissaTimesHundred) {
        return -1;
    } else if (target > next->mantissaTimesHundred) {
        return 1;
    }
    return 0;
}

/*!
 * \brief findBoundsForSuitableSeries
 * \details Finds two standard series by key in the series table.
 *          The desired value lies in the interval [ bounds[0] ; bounds[1] ).
 *          If the upper bound of the mantissa overflows during the search,
 *          bounds[0] is rewound to the beginning and the return carry flag is set to true.
 * \param key Search key.
 * \param bounds Array for two boundary values.
 * \param series Desired series.
 * \return Carry flag.
 */

static bool findBoundsForSuitableSeries(const IEC63StandartValueMantissa * key, const IEC63Std3SigFigsMantissaItem * bounds[2], IEC63StandartSeries series)
{
    assert(key);
    assert(*key < 1000);
    assert(bounds);
    assert(IEC63_STD_CORRECT_SERIES(series));
    assert(1u << series);
    bounds[0] = bsearch(key, mantissaLookup, ARRAY_SIZE(mantissaLookup), ELEMENT_SIZE(mantissaLookup), mantissaLookupRangeComparator);
    assert(bounds[0]);
    assert(bounds[0]->mantissaTimesHundred != 1000);

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

void iec63StandartValueInit(IEC63Standart3SigFigsValue * obj, double value, IEC63StandartSeries series, IEC63StandartRoundingPolicy rounding)
{
    //IEC63Standart3SigFigsValue notNormalized;
    IEC63StandartValueMantissa mantissaTimesHundred;
    IEC63StandartValueOrder order;
    const IEC63Std3SigFigsMantissaItem * bounds[2] = {NULL, NULL};
    bool carry;
    if (series == IEC63StandartSeriesNone) {
        series = IEC63StandartSeriesE24;
    }
    mantissaTimesHundred = convertFromDouble(value, &order);
    carry = findBoundsForSuitableSeries(&mantissaTimesHundred, bounds, series);
    obj->order = order;
    obj->series = series;

    switch (rounding) {
    case IEC63StandartRoundingPolicyCeil:
        if(bounds[0]->mantissaTimesHundred == mantissaTimesHundred) {
            // If the desired value is exactly equal to the standard one, then we accept it.
            obj->mantissaTimesHundred = mantissaTimesHundred;
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
            int delta_low = mantissaTimesHundred - bounds[0]->mantissaTimesHundred;
            int delta_high = bounds[1]->mantissaTimesHundred - mantissaTimesHundred;
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

double iec63StandartToDouble(const IEC63Standart3SigFigsValue * obj)
{
    return (double)obj->mantissaTimesHundred * pow(10, obj->order - 2);
}

void iec63StandartValueIncrement(IEC63Standart3SigFigsValue * obj)
{
    if (obj->mantissaTimesHundred == 0) return;

    IEC63StandartSeries targetSeries = obj->series;
    bool carry;
    if (targetSeries == IEC63StandartSeriesNone) {
        targetSeries = IEC63StandartSeriesE24;
    }
    const IEC63Std3SigFigsMantissaItem * bounds[2] = {NULL, NULL};
    carry = findBoundsForSuitableSeries(&obj->mantissaTimesHundred, bounds, targetSeries);
    if (carry) obj->order++;
    obj->mantissaTimesHundred = bounds[1]->mantissaTimesHundred;
}

void iec63StandartValueDecrement(IEC63Standart3SigFigsValue * obj)
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
    findBoundsForSuitableSeries(&obj->mantissaTimesHundred, bounds, targetSeries);
    obj->mantissaTimesHundred = bounds[0]->mantissaTimesHundred;
}

bool iec63StandartValueEq(const IEC63Standart3SigFigsValue * left, const IEC63Standart3SigFigsValue * right)
{
    return ( (left->order == right->order) && (left->mantissaTimesHundred == right->mantissaTimesHundred) );
}

bool iec63StandartValueGt(const IEC63Standart3SigFigsValue * left, const IEC63Standart3SigFigsValue * right)
{
    return ( (left->order >= right->order) && (left->mantissaTimesHundred > right->mantissaTimesHundred) );
}

void iec63StandartValueSetSeries(IEC63Standart3SigFigsValue * obj, IEC63StandartSeries series)
{
    if( obj->series == series ) return;
    const IEC63Std3SigFigsMantissaItem * bounds[2] = {NULL, NULL};
    bool carry;
    carry = findBoundsForSuitableSeries(&obj->mantissaTimesHundred, bounds, series);

    int delta_low = obj->mantissaTimesHundred - bounds[0]->mantissaTimesHundred;
    int delta_high = bounds[1]->mantissaTimesHundred - obj->mantissaTimesHundred;
    if(delta_low < delta_high) {
        obj->mantissaTimesHundred = bounds[0]->mantissaTimesHundred;
    } else {
        if (carry) obj->order++;
        obj->mantissaTimesHundred = bounds[1]->mantissaTimesHundred;
    }
}
