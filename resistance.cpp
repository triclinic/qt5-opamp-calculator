#include "resistance.h"
#include <QStringList>

StandartResistance::StandartResistance(double v, ResistanceSeries s) : m_value()
{
    standartValueDecompose(&m_value, v, (IEC63StandartSeries)s, IEC63StandartRoundingPolicyNearest);
}

StandartResistance& StandartResistance::operator++()
{
    standartValueIncrement(&m_value);
    return *this;
}

StandartResistance& StandartResistance::operator--()
{
    standartValueDecrement(&m_value);
    return *this;
}

bool StandartResistance::operator==(const StandartResistance & other) const
{
    return ( (m_value.order == other.m_value.order) && (m_value.mantissaTimesHundred == other.m_value.mantissaTimesHundred) );
}

bool StandartResistance::operator>(const StandartResistance & other) const
{
    return ( (m_value.order >= other.m_value.order) && (m_value.mantissaTimesHundred > other.m_value.mantissaTimesHundred) );
}

void StandartResistance::setSeries(ResistanceSeries s)
{
    standartValueSetSeries(&m_value, (IEC63StandartSeries) s);
}

ResistanceSeries StandartResistance::getSeries() const
{
    return (ResistanceSeries) m_value.series;
}

static const QStringList ResistanceSuffix = {"<LOW>", "mOhm", "Ohm", "KOhm", "MOhm", "GOhm", "<HIGH>"};

ResistanceRange StandartResistance::getRange() const
{
    if (m_value.order < -3) return ResistanceRange::TooLow;
    int shiftedOrder = m_value.order + 3;
    int range = shiftedOrder / 3;
    if (range >= (int)ResistanceRange::TooHigh) return ResistanceRange::TooHigh;
    return (ResistanceRange) ( range + (int)ResistanceRange::TooLow + 1 );
}

ResistanceDecade StandartResistance::getDecade() const
{
    if (m_value.order < 0) {
        int temp = -m_value.order % 3;
        if (!temp) return ResistanceDecade::Units;
        return (temp == 2) ? ResistanceDecade::Tens : ResistanceDecade::Hundreds;
    }
    return (ResistanceDecade) (m_value.order % 3);
}

StandartResistance StandartResistance::operator++(int)
{
    StandartResistance temp(*this);
    standartValueIncrement(&m_value);
    return temp;
}

StandartResistance StandartResistance::operator--(int)
{
    StandartResistance temp(*this);
    standartValueDecrement(&m_value);
    return temp;
}

double StandartResistance::value() const
{
    return standartValueCompose(&m_value);
}

QString StandartResistance::toString(ResistanceStringStyle style) const
{
    const ResistanceRange range = getRange();
    const ResistanceDecade decade = getDecade();
    if (range == ResistanceRange::TooLow) {
        return ResistanceSuffix[0];
    } else if (range == ResistanceRange::TooHigh) {
        return ResistanceSuffix[ResistanceSuffix.size() - 1];
    }
    bool isPrecise = ( getSeries() > ResistanceSeries::E24 );
    QString result, delimiter = ".";

    if ( style == ResistanceStringStyle::Scematic ) {
        const QString ResistanceDelimiter("?mRkMG?");
        delimiter = ResistanceDelimiter[(int)range];
    }
    int left, right;

    switch (decade) {
    case ResistanceDecade::Units:
        left = m_value.mantissaTimesHundred / 100;
        result = QString::number(left) + delimiter;
        if (isPrecise) {
            right = m_value.mantissaTimesHundred - left * 100;
        } else {
            right = (m_value.mantissaTimesHundred - left * 100) / 10;
        }
        result += QString::number(right);
        break;
    case ResistanceDecade::Tens:
        left = m_value.mantissaTimesHundred / 10;
        result = QString::number(left);
        if (isPrecise) {
            right = m_value.mantissaTimesHundred - left * 10;
            result += delimiter + QString::number(right);
        }
        break;
    case ResistanceDecade::Hundreds:
        result = QString::number(m_value.mantissaTimesHundred);
        break;
    default:
        result = "???";
        break;
    }
    if ( style == ResistanceStringStyle::Ordinary ) {
        result += " " + ResistanceSuffix[(int)range];
    } else if (range != ResistanceRange::Ohms) {
        result += delimiter;
    }
    return result;
}

double StandartResistance::standartize(double value, ResistanceSeries ser)
{
    IEC63Standart3SigFigsValue val = {};
    standartValueDecompose(&val, value, (IEC63StandartSeries)ser, IEC63StandartRoundingPolicyNearest);
    return standartValueCompose(&val);
}
