#ifndef RESISTANCE_H
#define RESISTANCE_H
#include <QString>
#include <memory>
#include "misc/series.h"

enum class ResistanceSeries {
    E3 = 1, E6, E12, E24, E48, E96, E192
};

enum class ResistanceRange {
    TooLow
    , MiliOhms
    , Ohms
    , KiloOhms
    , MegaOhms
    , GigaOhms
    , TooHigh
};

enum class ResistanceDecade {
    Units
    , Tens
    , Hundreds
};

enum class ResistanceStringStyle {
    Ordinary
    , Scematic
};

class StandartResistance
{
    IEC63Standart3SigFigsValue m_value;
    bool m_isNan;
public:
    explicit StandartResistance(double, ResistanceSeries = ResistanceSeries::E24);

    StandartResistance& operator++();
    StandartResistance operator++(int);
    StandartResistance& operator--();
    StandartResistance operator--(int);
    bool operator==(const StandartResistance&) const;
    bool operator!=(const StandartResistance& other) const { return !(*this == other); }
    bool operator>(const StandartResistance&) const;
    bool operator<=(const StandartResistance& other) const { return !(*this > other); }
    bool operator<(const StandartResistance& other) const { return !((*this > other) || (*this == other)); }
    bool operator>=(const StandartResistance& other) const { return ((*this > other) || (*this == other)); }

    void setSeries(ResistanceSeries);
    ResistanceSeries getSeries() const;
    ResistanceRange getRange() const;
    ResistanceDecade getDecade() const;
    double value() const;

    QString toString(ResistanceStringStyle = ResistanceStringStyle::Ordinary) const;
    static double standartize(double, ResistanceSeries = ResistanceSeries::E24);
};

#endif // RESISTANCE_H
