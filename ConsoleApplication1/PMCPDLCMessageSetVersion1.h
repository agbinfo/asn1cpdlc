// PMCPDLCMessageSetVersion1.h
//
#pragma once

#include <iterator>
#include <vector>
#include "Asn1.h"

namespace PMCPDLCMessageSetVersion1 {
typedef Asn1::Asn1Int<0, 63> MsgIdentificationNumber;
typedef Asn1::Asn1Int<0, 63> MsgReferenceNumber;
class AircraftAddress : public Asn1::BitString {
public:
  AircraftAddress() : BitString() { }
  AircraftAddress(int min, int max) : BitString(min, max) { }
  AircraftAddress(const AircraftAddress&src) : BitString(src) { }
  const AircraftAddress& operator= (const AircraftAddress&src) {
    return dynamic_cast<const AircraftAddress&>(BitString::operator=(src));
  }
  const AircraftAddress& operator= (const PrimitiveType v) {
    return dynamic_cast<const AircraftAddress&>(BitString::operator=(v));
  }
};
class AircraftFlightIdentification : public Asn1::Asn1IA5String<2, 8> {
public:
  AircraftFlightIdentification() : Asn1IA5String() { }
  AircraftFlightIdentification(const AircraftFlightIdentification&src) : Asn1IA5String(src) { }
};
class Airport : public Asn1::Asn1IA5String<4, 4> {
public:
  Airport() : Asn1IA5String() { }
  Airport(const Airport&src) : Asn1IA5String(src) { }
};
typedef Asn1::Asn1Int<2200, 3200> AltimeterEnglish;
typedef Asn1::Asn1Int<7500, 12500> AltimeterMetric;
class ATISCode : public Asn1::Asn1IA5String<1, 1> {
public:
  ATISCode() : Asn1IA5String() { }
  ATISCode(const ATISCode&src) : Asn1IA5String(src) { }
};
class ATSRouteDesignator : public Asn1::Asn1IA5String<2, 7> {
public:
  ATSRouteDesignator() : Asn1IA5String() { }
  ATSRouteDesignator(const ATSRouteDesignator&src) : Asn1IA5String(src) { }
};
typedef Asn1::Asn1Int<0, 7> CodeOctalDigit;
typedef Asn1::Asn1Int<1, 31> Day;
typedef Asn1::Asn1Int<1, 20> DegreeIncrement;
typedef Asn1::Asn1Int<1, 360> DegreesMagnetic;
typedef Asn1::Asn1Int<1, 360> DegreesTrue;
typedef Asn1::Asn1Int<1, 150> DepartureMinimumInterval;
typedef Asn1::Asn1Int<0, 8000> DistanceKm;
typedef Asn1::Asn1Int<0, 9999> DistanceNm;
typedef Asn1::Asn1Int<1, 500> DistanceSpecifiedKm;
typedef Asn1::Asn1Int<1, 250> DistanceSpecifiedNm;
class FacilityDesignation : public Asn1::Asn1IA5String<4, 8> {
public:
  FacilityDesignation() : Asn1IA5String() { }
  FacilityDesignation(const FacilityDesignation&src) : Asn1IA5String(src) { }
};
class FacilityName : public Asn1::Asn1IA5String<3, 18> {
public:
  FacilityName() : Asn1IA5String() { }
  FacilityName(const FacilityName&src) : Asn1IA5String(src) { }
};
class Fix : public Asn1::Asn1IA5String<1, 5> {
public:
  Fix() : Asn1IA5String() { }
  Fix(const Fix&src) : Asn1IA5String(src) { }
};
class FreeText : public Asn1::Asn1IA5String<1, 256> {
public:
  FreeText() : Asn1IA5String() { }
  FreeText(const FreeText&src) : Asn1IA5String(src) { }
};
typedef Asn1::Asn1Int<2850, 28000> Frequencyhf;
typedef Asn1::Asn1String<12, 12> Frequencysatchannel; // NumericString
typedef Asn1::Asn1Int<9000, 15999> Frequencyuhf;
typedef Asn1::Asn1Int<23600, 27398> Frequencyvhf;
typedef Asn1::Asn1Int<0, 100> Humidity;
typedef Asn1::Asn1Int<0, 90000> LatitudeDegrees;
typedef Asn1::Asn1Int<0, 89> LatitudeWholeDegrees;
typedef Asn1::Asn1Int<0, 59> LatLonWholeMinutes;
typedef Asn1::Asn1Int<0, 50> LegDistanceEnglish;
typedef Asn1::Asn1Int<1, 128> LegDistanceMetric;
typedef Asn1::Asn1Int<0, 10> LegTime;
typedef Asn1::Asn1Int<-60, 7000> LevelFeet;
typedef Asn1::Asn1Int<30, 700> LevelFlightLevel;
typedef Asn1::Asn1Int<100, 2500> LevelFlightLevelMetric;
typedef Asn1::Asn1Int<-30, 25000> LevelMeters;
typedef Asn1::Asn1Int<0, 180000> LongitudeDegrees;
typedef Asn1::Asn1Int<0, 179> LongitudeWholeDegrees;
typedef Asn1::Asn1Int<0, 5999> MinutesLatLon;
typedef Asn1::Asn1Int<1, 12> Month;
class NavaidName : public Asn1::Asn1IA5String<1, 4> {
public:
  NavaidName() : Asn1IA5String() { }
  NavaidName(const NavaidName&src) : Asn1IA5String(src) { }
};
typedef Asn1::Asn1Int<1, 1024> PersonsOnBoard;
class Procedure : public Asn1::Asn1IA5String<1, 20> {
public:
  Procedure() : Asn1IA5String() { }
  Procedure(const Procedure&src) : Asn1IA5String(src) { }
};
class ProcedureTransition : public Asn1::Asn1IA5String<1, 5> {
public:
  ProcedureTransition() : Asn1IA5String() { }
  ProcedureTransition(const ProcedureTransition&src) : Asn1IA5String(src) { }
};
typedef Asn1::Asn1Int<1, 16> RevisionNumber;
typedef Asn1::Asn1Int<1, 2> RouteClearanceIndex;
typedef Asn1::Asn1Int<1, 150> RTATolerance;
typedef Asn1::Asn1Int<1, 36> RunwayDirection;
typedef Asn1::Asn1Int<0, 6100> RVRFeet;
typedef Asn1::Asn1Int<0, 1500> RVRMeters;
typedef Asn1::Asn1Int<0, 59> SecondsLatLon;
typedef Asn1::Asn1Int<0, 400> SpeedIndicated;
typedef Asn1::Asn1Int<0, 800> SpeedIndicatedMetric;
typedef Asn1::Asn1Int<-50, 2000> SpeedGround;
typedef Asn1::Asn1Int<-100, 4000> SpeedGroundMetric;
typedef Asn1::Asn1Int<500, 4000> SpeedMach;
typedef Asn1::Asn1Int<0, 2000> SpeedTrue;
typedef Asn1::Asn1Int<0, 4000> SpeedTrueMetric;
typedef Asn1::Asn1Int<-100, 100> Temperature;
typedef Asn1::Asn1Int<0, 23> TimeHours;
typedef Asn1::Asn1Int<0, 59> TimeMinutes;
typedef Asn1::Asn1Int<0, 59> TimeSeconds;
typedef Asn1::Asn1Int<0, 15> VersionNumber;
typedef Asn1::Asn1Int<0, 3000> VerticalRateEnglish;
typedef Asn1::Asn1Int<0, 1000> VerticalRateMetric;
typedef Asn1::Asn1Int<1, 360> WindDirection;
typedef Asn1::Asn1Int<0, 255> WindSpeedEnglish;
typedef Asn1::Asn1Int<0, 511> WindSpeedMetric;
typedef Asn1::Asn1Int<1996, 2095> Year;
class ATCUplinkMessage;
class ATCUplinkMessageData;
class ATCDownlinkMessage;
class ATCDownlinkMessageData;
class ATCMessageHeader;
class LogicalAck;
class ATCUplinkMsgElementId;
class ATCDownlinkMsgElementId;
class Altimeter;
class ATWAlongTrackWaypoint;
class ATWLevel;
class ATWLevelTolerance;
class ATWDistance;
class ATWDistanceTolerance;
class ClearanceType;
class ControlledTime;
class Date;
class DateTimeGroup;
class Degrees;
class DepartureClearance;
class Direction;
class DirectionDegrees;
class Distance;
class DistanceSpecified;
class DistanceSpecifiedDirection;
class DistanceSpecifiedDirectionTime;
class ErrorInformation;
class Facility;
class FacilityFunction;
class FacilityDesignationAltimeter;
class FacilityDesignationATISCode;
class FixName;
class FlightInformation;
class Frequency;
class FurtherInstructions;
class Holdatwaypoint;
class HoldClearance;
class InterceptCourseFrom;
class InterceptCourseFromSelection;
class Icing;
class Latitude;
class LatitudeDegreesMinutes;
class LatitudeDegreesMinutesSeconds;
class LatitudeDirection;
class LatitudeLongitude;
class LatitudeReportingPoints;
class LatitudeType;
class LatLonReportingPoints;
class LegDistance;
class LegType;
class Level;
class LevelPosition;
class LevelProcedureName;
class LevelsOfFlight;
class LevelSpeed;
class LevelSpeedSpeed;
class LevelTime;
class LevelType;
class Longitude;
class LongitudeDegreesMinutes;
class LongitudeDegreesMinutesSeconds;
class LongitudeDirection;
class LongitudeReportingPoints;
class LongitudeType;
class Navaid;
class PlaceBearing;
class PlaceBearingDistance;
class Position;
class PositionDegrees;
class PositionDistanceSpecifiedDirection;
class PositionLevel;
class PositionLevelLevel;
class PositionLevelSpeed;
class PositionProcedureName;
class PositionReport;
class PositionRouteClearanceIndex;
class PositionSpeed;
class PositionSpeedSpeed;
class PositionTime;
class PositionTimeLevel;
class PositionTimeTime;
class PositionUnitNameFrequency;
class ProcedureName;
class ProcedureType;
class PublishedIdentifier;
class RemainingFuelPersonsOnBoard;
class ReportingPoints;
class RouteAndLevels;
class RouteClearance;
class RouteInformation;
class RouteInformationAdditional;
class RTARequiredTimeArrival;
class RTATime;
class Runway;
class RunwayConfiguration;
class RunwayRVR;
class RVR;
class Speed;
class SpeedTime;
class SpeedType;
class SpeedTypeSpeedTypeSpeedTypeSpeed;
class Time;
class TimeLevel;
class TimeDeparture;
class TimeDistanceSpecifiedDirection;
class TimeDistanceToFromPosition;
class Timehhmmss;
class TimeUnitNameFrequency;
class TimePosition;
class TimePositionLevel;
class TimePositionLevelSpeed;
class TimeSpeed;
class TimeSpeedSpeed;
class TimeToFromPosition;
class TimeTolerance;
class ToFrom;
class ToFromPosition;
class TrafficType;
class Turbulence;
class UnitName;
class UnitNameFrequency;
class VerticalChange;
class VerticalDirection;
class VerticalRate;
class WaypointSpeedLevel;
class Winds;
class WindSpeed;
typedef Asn1::SequenceOf<ATWLevel,1,2> ATWLevelSequence;
typedef Asn1::SequenceOf<CodeOctalDigit,4,4> Code;
typedef Asn1::SequenceOf<Level,2,2> LevelLevel;
typedef Asn1::SequenceOf<PlaceBearing,2,2> PlaceBearingPlaceBearing;
typedef Asn1::SequenceOf<Position,2,2> PositionPosition;
typedef Time RemainingFuel;
typedef Asn1::SequenceOf<Speed,2,2> SpeedSpeed;
typedef Asn1::SequenceOf<SpeedType,3,3> SpeedTypeSpeedTypeSpeedType;
typedef Asn1::SequenceOf<Time,2,2> TimeTime;

class ATCUplinkMessage : public Asn1::Sequence {
public:
  Required<ATCMessageHeader>::type header;
  Required<ATCUplinkMessageData>::type messageData;
  ATCUplinkMessage() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << header << messageData;
  }
};

class ATCUplinkMessageData : public Asn1::Sequence {
public:
  Required<Asn1::SequenceOf<ATCUplinkMsgElementId, 1, 5> >::type elementIds;
  class InnerSeq_1 : public Asn1::Sequence {
  public:
    Optional<Asn1::SequenceOf<RouteClearance, 1, 2> >::type routeClearanceData;
    InnerSeq_1() : Sequence(1) {
      Sequence::Inserter it = back_inserter();
      it << routeClearanceData;
    }
  };
  
  
  Optional<InnerSeq_1 >::type constrainedData;
  ATCUplinkMessageData() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << elementIds << constrainedData;
  }
};

class ATCDownlinkMessage : public Asn1::Sequence {
public:
  Required<ATCMessageHeader>::type header;
  Required<ATCDownlinkMessageData>::type messageData;
  ATCDownlinkMessage() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << header << messageData;
  }
};

class ATCDownlinkMessageData : public Asn1::Sequence {
public:
  Required<Asn1::SequenceOf<ATCDownlinkMsgElementId, 1, 5> >::type elementIds;
  class InnerSeq_1 : public Asn1::Sequence {
  public:
    Optional<Asn1::SequenceOf<RouteClearance, 1, 2> >::type routeClearanceData;
    InnerSeq_1() : Sequence(1) {
      Sequence::Inserter it = back_inserter();
      it << routeClearanceData;
    }
  };
  
  
  Optional<InnerSeq_1 >::type constrainedData;
  ATCDownlinkMessageData() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << elementIds << constrainedData;
  }
};

class ATCMessageHeader : public Asn1::Sequence {
public:
  Required<MsgIdentificationNumber>::type messageIdNumber;
  Optional<MsgReferenceNumber>::type messageRefNumber;
  Required<DateTimeGroup>::type dateTime;
  Required<LogicalAck>::type logicalAck;
  ATCMessageHeader() : Sequence(4) {
    Sequence::Inserter it = back_inserter();
    it << messageIdNumber << messageRefNumber << dateTime << logicalAck;
  }
};

class LogicalAck : public Asn1::Enumerated<0,1> {
public:
  virtual bool extensible() const { return false; }
  typedef enum {
    required = 0,
    notRequired = 1
  } EnumValues;
  LogicalAck() : Enumerated() { }
  LogicalAck(int value) : Enumerated(value) { }
};

class ATCUplinkMsgElementId : public Asn1::Choice {
public:
  typedef ATCUplinkMsgElementId PrimitiveType;
  enum {
    uM0NULL_idx = 0,
    uM1NULL_idx = 1,
    uM2NULL_idx = 2,
    uM3NULL_idx = 3,
    uM4NULL_idx = 4,
    uM5NULL_idx = 5,
    uM6Level_idx = 6,
    uM7Time_idx = 7,
    uM8Position_idx = 8,
    uM9Time_idx = 9,
    uM10Position_idx = 10,
    uM11Time_idx = 11,
    uM12Position_idx = 12,
    uM13TimeLevel_idx = 13,
    uM14PositionLevel_idx = 14,
    uM15TimeLevel_idx = 15,
    uM16PositionLevel_idx = 16,
    uM17TimeLevel_idx = 17,
    uM18PositionLevel_idx = 18,
    uM19Level_idx = 19,
    uM20Level_idx = 20,
    uM21TimeLevel_idx = 21,
    uM22PositionLevel_idx = 22,
    uM23Level_idx = 23,
    uM24TimeLevel_idx = 24,
    uM25PositionLevel_idx = 25,
    uM26LevelTime_idx = 26,
    uM27LevelPosition_idx = 27,
    uM28LevelTime_idx = 28,
    uM29LevelPosition_idx = 29,
    uM30LevelLevel_idx = 30,
    uM31LevelLevel_idx = 31,
    uM32LevelLevel_idx = 32,
    uM33NULL_idx = 33,
    uM34Level_idx = 34,
    uM35Level_idx = 35,
    uM36Level_idx = 36,
    uM37Level_idx = 37,
    uM38Level_idx = 38,
    uM39Level_idx = 39,
    uM40NULL_idx = 40,
    uM41NULL_idx = 41,
    uM42PositionLevel_idx = 42,
    uM43PositionLevel_idx = 43,
    uM44PositionLevel_idx = 44,
    uM45PositionLevel_idx = 45,
    uM46PositionLevel_idx = 46,
    uM47PositionLevel_idx = 47,
    uM48PositionLevel_idx = 48,
    uM49PositionLevel_idx = 49,
    uM50PositionLevelLevel_idx = 50,
    uM51PositionTime_idx = 51,
    uM52PositionTime_idx = 52,
    uM53PositionTime_idx = 53,
    uM54PositionTimeTime_idx = 54,
    uM55PositionSpeed_idx = 55,
    uM56PositionSpeed_idx = 56,
    uM57PositionSpeed_idx = 57,
    uM58PositionTimeLevel_idx = 58,
    uM59PositionTimeLevel_idx = 59,
    uM60PositionTimeLevel_idx = 60,
    uM61PositionLevelSpeed_idx = 61,
    uM62TimePositionLevel_idx = 62,
    uM63TimePositionLevelSpeed_idx = 63,
    uM64DistanceSpecifiedDirection_idx = 64,
    uM65PositionDistanceSpecifiedDirection_idx = 65,
    uM66TimeDistanceSpecifiedDirection_idx = 66,
    uM67NULL_idx = 67,
    uM68Position_idx = 68,
    uM69Time_idx = 69,
    uM70Position_idx = 70,
    uM71Time_idx = 71,
    uM72NULL_idx = 72,
    uM73DepartureClearance_idx = 73,
    uM74Position_idx = 74,
    uM75Position_idx = 75,
    uM76TimePosition_idx = 76,
    uM77PositionPosition_idx = 77,
    uM78LevelPosition_idx = 78,
    uM79PositionRouteClearance_idx = 79,
    uM80RouteClearance_idx = 80,
    uM81ProcedureName_idx = 81,
    uM82DistanceSpecifiedDirection_idx = 82,
    uM83PositionRouteClearance_idx = 83,
    uM84PositionProcedureName_idx = 84,
    uM85RouteClearance_idx = 85,
    uM86PositionRouteClearance_idx = 86,
    uM87Position_idx = 87,
    uM88PositionPosition_idx = 88,
    uM89TimePosition_idx = 89,
    uM90LevelPosition_idx = 90,
    uM91HoldClearance_idx = 91,
    uM92PositionLevel_idx = 92,
    uM93Time_idx = 93,
    uM94DirectionDegrees_idx = 94,
    uM95DirectionDegrees_idx = 95,
    uM96NULL_idx = 96,
    uM97PositionDegrees_idx = 97,
    uM98DirectionDegrees_idx = 98,
    uM99ProcedureName_idx = 99,
    uM100TimeSpeed_idx = 100,
    uM101PositionSpeed_idx = 101,
    uM102LevelSpeed_idx = 102,
    uM103TimeSpeedSpeed_idx = 103,
    uM104PositionSpeedSpeed_idx = 104,
    uM105LevelSpeedSpeed_idx = 105,
    uM106Speed_idx = 106,
    uM107NULL_idx = 107,
    uM108Speed_idx = 108,
    uM109Speed_idx = 109,
    uM110SpeedSpeed_idx = 110,
    uM111Speed_idx = 111,
    uM112Speed_idx = 112,
    uM113Speed_idx = 113,
    uM114Speed_idx = 114,
    uM115Speed_idx = 115,
    uM116NULL_idx = 116,
    uM117UnitNameFrequency_idx = 117,
    uM118PositionUnitNameFrequency_idx = 118,
    uM119TimeUnitNameFrequency_idx = 119,
    uM120UnitNameFrequency_idx = 120,
    uM121PositionUnitNameFrequency_idx = 121,
    uM122TimeUnitNameFrequency_idx = 122,
    uM123Code_idx = 123,
    uM124NULL_idx = 124,
    uM125NULL_idx = 125,
    uM126NULL_idx = 126,
    uM127NULL_idx = 127,
    uM128Level_idx = 128,
    uM129Level_idx = 129,
    uM130Position_idx = 130,
    uM131NULL_idx = 131,
    uM132NULL_idx = 132,
    uM133NULL_idx = 133,
    uM134SpeedTypeSpeedTypeSpeedType_idx = 134,
    uM135NULL_idx = 135,
    uM136NULL_idx = 136,
    uM137NULL_idx = 137,
    uM138NULL_idx = 138,
    uM139NULL_idx = 139,
    uM140NULL_idx = 140,
    uM141NULL_idx = 141,
    uM142NULL_idx = 142,
    uM143NULL_idx = 143,
    uM144NULL_idx = 144,
    uM145NULL_idx = 145,
    uM146NULL_idx = 146,
    uM147NULL_idx = 147,
    uM148Level_idx = 148,
    uM149LevelPosition_idx = 149,
    uM150LevelTime_idx = 150,
    uM151Speed_idx = 151,
    uM152DistanceSpecifiedDirection_idx = 152,
    uM153Altimeter_idx = 153,
    uM154NULL_idx = 154,
    uM155Position_idx = 155,
    uM156NULL_idx = 156,
    uM157Frequency_idx = 157,
    uM158AtisCode_idx = 158,
    uM159ErrorInformation_idx = 159,
    uM160Facility_idx = 160,
    uM161NULL_idx = 161,
    uM162NULL_idx = 162,
    uM163FacilityDesignation_idx = 163,
    uM164NULL_idx = 164,
    uM165NULL_idx = 165,
    uM166TrafficType_idx = 166,
    uM167NULL_idx = 167,
    uM168NULL_idx = 168,
    uM169FreeText_idx = 169,
    uM170FreeText_idx = 170,
    uM171VerticalRate_idx = 171,
    uM172VerticalRate_idx = 172,
    uM173VerticalRate_idx = 173,
    uM174VerticalRate_idx = 174,
    uM175Level_idx = 175,
    uM176NULL_idx = 176,
    uM177NULL_idx = 177,
    uM178NULL_idx = 178,
    uM179NULL_idx = 179,
    uM180LevelLevel_idx = 180,
    uM181ToFromPosition_idx = 181,
    uM182NULL_idx = 182,
    uM183FreeText_idx = 183,
    uM184TimeToFromPosition_idx = 184,
    uM185PositionLevel_idx = 185,
    uM186PositionLevel_idx = 186,
    uM187FreeText_idx = 187,
    uM188PositionSpeed_idx = 188,
    uM189Speed_idx = 189,
    uM190Degrees_idx = 190,
    uM191NULL_idx = 191,
    uM192LevelTime_idx = 192,
    uM193NULL_idx = 193,
    uM194FreeText_idx = 194,
    uM195FreeText_idx = 195,
    uM196FreeText_idx = 196,
    uM197FreeText_idx = 197,
    uM198FreeText_idx = 198,
    uM199FreeText_idx = 199,
    uM200NULL_idx = 200,
    uM201NULL_idx = 201,
    uM202NULL_idx = 202,
    uM203FreeText_idx = 203,
    uM204FreeText_idx = 204,
    uM205FreeText_idx = 205,
    uM206FreeText_idx = 206,
    uM207FreeText_idx = 207,
    uM208FreeText_idx = 208,
    uM209LevelPosition_idx = 209,
    uM210Position_idx = 210,
    uM211NULL_idx = 211,
    uM212FacilityDesignationATISCode_idx = 212,
    uM213FacilityDesignationAltimeter_idx = 213,
    uM214RunwayRVR_idx = 214,
    uM215DirectionDegrees_idx = 215,
    uM216NULL_idx = 216,
    uM217NULL_idx = 217,
    uM218NULL_idx = 218,
    uM219Level_idx = 219,
    uM220Level_idx = 220,
    uM221Degrees_idx = 221,
    uM222NULL_idx = 222,
    uM223NULL_idx = 223,
    uM224NULL_idx = 224,
    uM225NULL_idx = 225,
    uM226Time_idx = 226,
    uM227NULL_idx = 227,
    uM228Position_idx = 228,
    uM229NULL_idx = 229,
    uM230NULL_idx = 230,
    uM231NULL_idx = 231,
    uM232NULL_idx = 232,
    uM233NULL_idx = 233,
    uM234NULL_idx = 234,
    uM235NULL_idx = 235,
    uM236NULL_idx = 236,
    uM237NULL_idx = 237
  };
  virtual bool extensible() const { return true; }
  Option<uM0NULL_idx, Asn1::Asn1NULL>::type uM0NULL();
  Option<uM1NULL_idx, Asn1::Asn1NULL>::type uM1NULL();
  Option<uM2NULL_idx, Asn1::Asn1NULL>::type uM2NULL();
  Option<uM3NULL_idx, Asn1::Asn1NULL>::type uM3NULL();
  Option<uM4NULL_idx, Asn1::Asn1NULL>::type uM4NULL();
  Option<uM5NULL_idx, Asn1::Asn1NULL>::type uM5NULL();
  Option<uM6Level_idx, Level>::type uM6Level();
  Option<uM7Time_idx, Time>::type uM7Time();
  Option<uM8Position_idx, Position>::type uM8Position();
  Option<uM9Time_idx, Time>::type uM9Time();
  Option<uM10Position_idx, Position>::type uM10Position();
  Option<uM11Time_idx, Time>::type uM11Time();
  Option<uM12Position_idx, Position>::type uM12Position();
  Option<uM13TimeLevel_idx, TimeLevel>::type uM13TimeLevel();
  Option<uM14PositionLevel_idx, PositionLevel>::type uM14PositionLevel();
  Option<uM15TimeLevel_idx, TimeLevel>::type uM15TimeLevel();
  Option<uM16PositionLevel_idx, PositionLevel>::type uM16PositionLevel();
  Option<uM17TimeLevel_idx, TimeLevel>::type uM17TimeLevel();
  Option<uM18PositionLevel_idx, PositionLevel>::type uM18PositionLevel();
  Option<uM19Level_idx, Level>::type uM19Level();
  Option<uM20Level_idx, Level>::type uM20Level();
  Option<uM21TimeLevel_idx, TimeLevel>::type uM21TimeLevel();
  Option<uM22PositionLevel_idx, PositionLevel>::type uM22PositionLevel();
  Option<uM23Level_idx, Level>::type uM23Level();
  Option<uM24TimeLevel_idx, TimeLevel>::type uM24TimeLevel();
  Option<uM25PositionLevel_idx, PositionLevel>::type uM25PositionLevel();
  Option<uM26LevelTime_idx, LevelTime>::type uM26LevelTime();
  Option<uM27LevelPosition_idx, LevelPosition>::type uM27LevelPosition();
  Option<uM28LevelTime_idx, LevelTime>::type uM28LevelTime();
  Option<uM29LevelPosition_idx, LevelPosition>::type uM29LevelPosition();
  Option<uM30LevelLevel_idx, LevelLevel>::type uM30LevelLevel();
  Option<uM31LevelLevel_idx, LevelLevel>::type uM31LevelLevel();
  Option<uM32LevelLevel_idx, LevelLevel>::type uM32LevelLevel();
  Option<uM33NULL_idx, Asn1::Asn1NULL>::type uM33NULL();
  Option<uM34Level_idx, Level>::type uM34Level();
  Option<uM35Level_idx, Level>::type uM35Level();
  Option<uM36Level_idx, Level>::type uM36Level();
  Option<uM37Level_idx, Level>::type uM37Level();
  Option<uM38Level_idx, Level>::type uM38Level();
  Option<uM39Level_idx, Level>::type uM39Level();
  Option<uM40NULL_idx, Asn1::Asn1NULL>::type uM40NULL();
  Option<uM41NULL_idx, Asn1::Asn1NULL>::type uM41NULL();
  Option<uM42PositionLevel_idx, PositionLevel>::type uM42PositionLevel();
  Option<uM43PositionLevel_idx, PositionLevel>::type uM43PositionLevel();
  Option<uM44PositionLevel_idx, PositionLevel>::type uM44PositionLevel();
  Option<uM45PositionLevel_idx, PositionLevel>::type uM45PositionLevel();
  Option<uM46PositionLevel_idx, PositionLevel>::type uM46PositionLevel();
  Option<uM47PositionLevel_idx, PositionLevel>::type uM47PositionLevel();
  Option<uM48PositionLevel_idx, PositionLevel>::type uM48PositionLevel();
  Option<uM49PositionLevel_idx, PositionLevel>::type uM49PositionLevel();
  Option<uM50PositionLevelLevel_idx, PositionLevelLevel>::type uM50PositionLevelLevel();
  Option<uM51PositionTime_idx, PositionTime>::type uM51PositionTime();
  Option<uM52PositionTime_idx, PositionTime>::type uM52PositionTime();
  Option<uM53PositionTime_idx, PositionTime>::type uM53PositionTime();
  Option<uM54PositionTimeTime_idx, PositionTimeTime>::type uM54PositionTimeTime();
  Option<uM55PositionSpeed_idx, PositionSpeed>::type uM55PositionSpeed();
  Option<uM56PositionSpeed_idx, PositionSpeed>::type uM56PositionSpeed();
  Option<uM57PositionSpeed_idx, PositionSpeed>::type uM57PositionSpeed();
  Option<uM58PositionTimeLevel_idx, PositionTimeLevel>::type uM58PositionTimeLevel();
  Option<uM59PositionTimeLevel_idx, PositionTimeLevel>::type uM59PositionTimeLevel();
  Option<uM60PositionTimeLevel_idx, PositionTimeLevel>::type uM60PositionTimeLevel();
  Option<uM61PositionLevelSpeed_idx, PositionLevelSpeed>::type uM61PositionLevelSpeed();
  Option<uM62TimePositionLevel_idx, TimePositionLevel>::type uM62TimePositionLevel();
  Option<uM63TimePositionLevelSpeed_idx, TimePositionLevelSpeed>::type uM63TimePositionLevelSpeed();
  Option<uM64DistanceSpecifiedDirection_idx, DistanceSpecifiedDirection>::type uM64DistanceSpecifiedDirection();
  Option<uM65PositionDistanceSpecifiedDirection_idx, PositionDistanceSpecifiedDirection>::type uM65PositionDistanceSpecifiedDirection();
  Option<uM66TimeDistanceSpecifiedDirection_idx, TimeDistanceSpecifiedDirection>::type uM66TimeDistanceSpecifiedDirection();
  Option<uM67NULL_idx, Asn1::Asn1NULL>::type uM67NULL();
  Option<uM68Position_idx, Position>::type uM68Position();
  Option<uM69Time_idx, Time>::type uM69Time();
  Option<uM70Position_idx, Position>::type uM70Position();
  Option<uM71Time_idx, Time>::type uM71Time();
  Option<uM72NULL_idx, Asn1::Asn1NULL>::type uM72NULL();
  Option<uM73DepartureClearance_idx, DepartureClearance>::type uM73DepartureClearance();
  Option<uM74Position_idx, Position>::type uM74Position();
  Option<uM75Position_idx, Position>::type uM75Position();
  Option<uM76TimePosition_idx, TimePosition>::type uM76TimePosition();
  Option<uM77PositionPosition_idx, PositionPosition>::type uM77PositionPosition();
  Option<uM78LevelPosition_idx, LevelPosition>::type uM78LevelPosition();
  Option<uM79PositionRouteClearance_idx, PositionRouteClearanceIndex>::type uM79PositionRouteClearance();
  Option<uM80RouteClearance_idx, RouteClearanceIndex>::type uM80RouteClearance();
  Option<uM81ProcedureName_idx, ProcedureName>::type uM81ProcedureName();
  Option<uM82DistanceSpecifiedDirection_idx, DistanceSpecifiedDirection>::type uM82DistanceSpecifiedDirection();
  Option<uM83PositionRouteClearance_idx, PositionRouteClearanceIndex>::type uM83PositionRouteClearance();
  Option<uM84PositionProcedureName_idx, PositionProcedureName>::type uM84PositionProcedureName();
  Option<uM85RouteClearance_idx, RouteClearanceIndex>::type uM85RouteClearance();
  Option<uM86PositionRouteClearance_idx, PositionRouteClearanceIndex>::type uM86PositionRouteClearance();
  Option<uM87Position_idx, Position>::type uM87Position();
  Option<uM88PositionPosition_idx, PositionPosition>::type uM88PositionPosition();
  Option<uM89TimePosition_idx, TimePosition>::type uM89TimePosition();
  Option<uM90LevelPosition_idx, LevelPosition>::type uM90LevelPosition();
  Option<uM91HoldClearance_idx, HoldClearance>::type uM91HoldClearance();
  Option<uM92PositionLevel_idx, PositionLevel>::type uM92PositionLevel();
  Option<uM93Time_idx, Time>::type uM93Time();
  Option<uM94DirectionDegrees_idx, DirectionDegrees>::type uM94DirectionDegrees();
  Option<uM95DirectionDegrees_idx, DirectionDegrees>::type uM95DirectionDegrees();
  Option<uM96NULL_idx, Asn1::Asn1NULL>::type uM96NULL();
  Option<uM97PositionDegrees_idx, PositionDegrees>::type uM97PositionDegrees();
  Option<uM98DirectionDegrees_idx, DirectionDegrees>::type uM98DirectionDegrees();
  Option<uM99ProcedureName_idx, ProcedureName>::type uM99ProcedureName();
  Option<uM100TimeSpeed_idx, TimeSpeed>::type uM100TimeSpeed();
  Option<uM101PositionSpeed_idx, PositionSpeed>::type uM101PositionSpeed();
  Option<uM102LevelSpeed_idx, LevelSpeed>::type uM102LevelSpeed();
  Option<uM103TimeSpeedSpeed_idx, TimeSpeedSpeed>::type uM103TimeSpeedSpeed();
  Option<uM104PositionSpeedSpeed_idx, PositionSpeedSpeed>::type uM104PositionSpeedSpeed();
  Option<uM105LevelSpeedSpeed_idx, LevelSpeedSpeed>::type uM105LevelSpeedSpeed();
  Option<uM106Speed_idx, Speed>::type uM106Speed();
  Option<uM107NULL_idx, Asn1::Asn1NULL>::type uM107NULL();
  Option<uM108Speed_idx, Speed>::type uM108Speed();
  Option<uM109Speed_idx, Speed>::type uM109Speed();
  Option<uM110SpeedSpeed_idx, SpeedSpeed>::type uM110SpeedSpeed();
  Option<uM111Speed_idx, Speed>::type uM111Speed();
  Option<uM112Speed_idx, Speed>::type uM112Speed();
  Option<uM113Speed_idx, Speed>::type uM113Speed();
  Option<uM114Speed_idx, Speed>::type uM114Speed();
  Option<uM115Speed_idx, Speed>::type uM115Speed();
  Option<uM116NULL_idx, Asn1::Asn1NULL>::type uM116NULL();
  Option<uM117UnitNameFrequency_idx, UnitNameFrequency>::type uM117UnitNameFrequency();
  Option<uM118PositionUnitNameFrequency_idx, PositionUnitNameFrequency>::type uM118PositionUnitNameFrequency();
  Option<uM119TimeUnitNameFrequency_idx, TimeUnitNameFrequency>::type uM119TimeUnitNameFrequency();
  Option<uM120UnitNameFrequency_idx, UnitNameFrequency>::type uM120UnitNameFrequency();
  Option<uM121PositionUnitNameFrequency_idx, PositionUnitNameFrequency>::type uM121PositionUnitNameFrequency();
  Option<uM122TimeUnitNameFrequency_idx, TimeUnitNameFrequency>::type uM122TimeUnitNameFrequency();
  Option<uM123Code_idx, Code>::type uM123Code();
  Option<uM124NULL_idx, Asn1::Asn1NULL>::type uM124NULL();
  Option<uM125NULL_idx, Asn1::Asn1NULL>::type uM125NULL();
  Option<uM126NULL_idx, Asn1::Asn1NULL>::type uM126NULL();
  Option<uM127NULL_idx, Asn1::Asn1NULL>::type uM127NULL();
  Option<uM128Level_idx, Level>::type uM128Level();
  Option<uM129Level_idx, Level>::type uM129Level();
  Option<uM130Position_idx, Position>::type uM130Position();
  Option<uM131NULL_idx, Asn1::Asn1NULL>::type uM131NULL();
  Option<uM132NULL_idx, Asn1::Asn1NULL>::type uM132NULL();
  Option<uM133NULL_idx, Asn1::Asn1NULL>::type uM133NULL();
  Option<uM134SpeedTypeSpeedTypeSpeedType_idx, SpeedTypeSpeedTypeSpeedType>::type uM134SpeedTypeSpeedTypeSpeedType();
  Option<uM135NULL_idx, Asn1::Asn1NULL>::type uM135NULL();
  Option<uM136NULL_idx, Asn1::Asn1NULL>::type uM136NULL();
  Option<uM137NULL_idx, Asn1::Asn1NULL>::type uM137NULL();
  Option<uM138NULL_idx, Asn1::Asn1NULL>::type uM138NULL();
  Option<uM139NULL_idx, Asn1::Asn1NULL>::type uM139NULL();
  Option<uM140NULL_idx, Asn1::Asn1NULL>::type uM140NULL();
  Option<uM141NULL_idx, Asn1::Asn1NULL>::type uM141NULL();
  Option<uM142NULL_idx, Asn1::Asn1NULL>::type uM142NULL();
  Option<uM143NULL_idx, Asn1::Asn1NULL>::type uM143NULL();
  Option<uM144NULL_idx, Asn1::Asn1NULL>::type uM144NULL();
  Option<uM145NULL_idx, Asn1::Asn1NULL>::type uM145NULL();
  Option<uM146NULL_idx, Asn1::Asn1NULL>::type uM146NULL();
  Option<uM147NULL_idx, Asn1::Asn1NULL>::type uM147NULL();
  Option<uM148Level_idx, Level>::type uM148Level();
  Option<uM149LevelPosition_idx, LevelPosition>::type uM149LevelPosition();
  Option<uM150LevelTime_idx, LevelTime>::type uM150LevelTime();
  Option<uM151Speed_idx, Speed>::type uM151Speed();
  Option<uM152DistanceSpecifiedDirection_idx, DistanceSpecifiedDirection>::type uM152DistanceSpecifiedDirection();
  Option<uM153Altimeter_idx, Altimeter>::type uM153Altimeter();
  Option<uM154NULL_idx, Asn1::Asn1NULL>::type uM154NULL();
  Option<uM155Position_idx, Position>::type uM155Position();
  Option<uM156NULL_idx, Asn1::Asn1NULL>::type uM156NULL();
  Option<uM157Frequency_idx, Frequency>::type uM157Frequency();
  Option<uM158AtisCode_idx, ATISCode>::type uM158AtisCode();
  Option<uM159ErrorInformation_idx, ErrorInformation>::type uM159ErrorInformation();
  Option<uM160Facility_idx, Facility>::type uM160Facility();
  Option<uM161NULL_idx, Asn1::Asn1NULL>::type uM161NULL();
  Option<uM162NULL_idx, Asn1::Asn1NULL>::type uM162NULL();
  Option<uM163FacilityDesignation_idx, FacilityDesignation>::type uM163FacilityDesignation();
  Option<uM164NULL_idx, Asn1::Asn1NULL>::type uM164NULL();
  Option<uM165NULL_idx, Asn1::Asn1NULL>::type uM165NULL();
  Option<uM166TrafficType_idx, TrafficType>::type uM166TrafficType();
  Option<uM167NULL_idx, Asn1::Asn1NULL>::type uM167NULL();
  Option<uM168NULL_idx, Asn1::Asn1NULL>::type uM168NULL();
  Option<uM169FreeText_idx, FreeText>::type uM169FreeText();
  Option<uM170FreeText_idx, FreeText>::type uM170FreeText();
  Option<uM171VerticalRate_idx, VerticalRate>::type uM171VerticalRate();
  Option<uM172VerticalRate_idx, VerticalRate>::type uM172VerticalRate();
  Option<uM173VerticalRate_idx, VerticalRate>::type uM173VerticalRate();
  Option<uM174VerticalRate_idx, VerticalRate>::type uM174VerticalRate();
  Option<uM175Level_idx, Level>::type uM175Level();
  Option<uM176NULL_idx, Asn1::Asn1NULL>::type uM176NULL();
  Option<uM177NULL_idx, Asn1::Asn1NULL>::type uM177NULL();
  Option<uM178NULL_idx, Asn1::Asn1NULL>::type uM178NULL();
  Option<uM179NULL_idx, Asn1::Asn1NULL>::type uM179NULL();
  Option<uM180LevelLevel_idx, LevelLevel>::type uM180LevelLevel();
  Option<uM181ToFromPosition_idx, ToFromPosition>::type uM181ToFromPosition();
  Option<uM182NULL_idx, Asn1::Asn1NULL>::type uM182NULL();
  Option<uM183FreeText_idx, FreeText>::type uM183FreeText();
  Option<uM184TimeToFromPosition_idx, TimeToFromPosition>::type uM184TimeToFromPosition();
  Option<uM185PositionLevel_idx, PositionLevel>::type uM185PositionLevel();
  Option<uM186PositionLevel_idx, PositionLevel>::type uM186PositionLevel();
  Option<uM187FreeText_idx, FreeText>::type uM187FreeText();
  Option<uM188PositionSpeed_idx, PositionSpeed>::type uM188PositionSpeed();
  Option<uM189Speed_idx, Speed>::type uM189Speed();
  Option<uM190Degrees_idx, Degrees>::type uM190Degrees();
  Option<uM191NULL_idx, Asn1::Asn1NULL>::type uM191NULL();
  Option<uM192LevelTime_idx, LevelTime>::type uM192LevelTime();
  Option<uM193NULL_idx, Asn1::Asn1NULL>::type uM193NULL();
  Option<uM194FreeText_idx, FreeText>::type uM194FreeText();
  Option<uM195FreeText_idx, FreeText>::type uM195FreeText();
  Option<uM196FreeText_idx, FreeText>::type uM196FreeText();
  Option<uM197FreeText_idx, FreeText>::type uM197FreeText();
  Option<uM198FreeText_idx, FreeText>::type uM198FreeText();
  Option<uM199FreeText_idx, FreeText>::type uM199FreeText();
  Option<uM200NULL_idx, Asn1::Asn1NULL>::type uM200NULL();
  Option<uM201NULL_idx, Asn1::Asn1NULL>::type uM201NULL();
  Option<uM202NULL_idx, Asn1::Asn1NULL>::type uM202NULL();
  Option<uM203FreeText_idx, FreeText>::type uM203FreeText();
  Option<uM204FreeText_idx, FreeText>::type uM204FreeText();
  Option<uM205FreeText_idx, FreeText>::type uM205FreeText();
  Option<uM206FreeText_idx, FreeText>::type uM206FreeText();
  Option<uM207FreeText_idx, FreeText>::type uM207FreeText();
  Option<uM208FreeText_idx, FreeText>::type uM208FreeText();
  Option<uM209LevelPosition_idx, LevelPosition>::type uM209LevelPosition();
  Option<uM210Position_idx, Position>::type uM210Position();
  Option<uM211NULL_idx, Asn1::Asn1NULL>::type uM211NULL();
  Option<uM212FacilityDesignationATISCode_idx, FacilityDesignationATISCode>::type uM212FacilityDesignationATISCode();
  Option<uM213FacilityDesignationAltimeter_idx, FacilityDesignationAltimeter>::type uM213FacilityDesignationAltimeter();
  Option<uM214RunwayRVR_idx, RunwayRVR>::type uM214RunwayRVR();
  Option<uM215DirectionDegrees_idx, DirectionDegrees>::type uM215DirectionDegrees();
  Option<uM216NULL_idx, Asn1::Asn1NULL>::type uM216NULL();
  Option<uM217NULL_idx, Asn1::Asn1NULL>::type uM217NULL();
  Option<uM218NULL_idx, Asn1::Asn1NULL>::type uM218NULL();
  Option<uM219Level_idx, Level>::type uM219Level();
  Option<uM220Level_idx, Level>::type uM220Level();
  Option<uM221Degrees_idx, Degrees>::type uM221Degrees();
  Option<uM222NULL_idx, Asn1::Asn1NULL>::type uM222NULL();
  Option<uM223NULL_idx, Asn1::Asn1NULL>::type uM223NULL();
  Option<uM224NULL_idx, Asn1::Asn1NULL>::type uM224NULL();
  Option<uM225NULL_idx, Asn1::Asn1NULL>::type uM225NULL();
  Option<uM226Time_idx, Time>::type uM226Time();
  Option<uM227NULL_idx, Asn1::Asn1NULL>::type uM227NULL();
  Option<uM228Position_idx, Position>::type uM228Position();
  Option<uM229NULL_idx, Asn1::Asn1NULL>::type uM229NULL();
  Option<uM230NULL_idx, Asn1::Asn1NULL>::type uM230NULL();
  Option<uM231NULL_idx, Asn1::Asn1NULL>::type uM231NULL();
  Option<uM232NULL_idx, Asn1::Asn1NULL>::type uM232NULL();
  Option<uM233NULL_idx, Asn1::Asn1NULL>::type uM233NULL();
  Option<uM234NULL_idx, Asn1::Asn1NULL>::type uM234NULL();
  Option<uM235NULL_idx, Asn1::Asn1NULL>::type uM235NULL();
  Option<uM236NULL_idx, Asn1::Asn1NULL>::type uM236NULL();
  Option<uM237NULL_idx, Asn1::Asn1NULL>::type uM237NULL();
  ATCUplinkMsgElementId() : Choice() { }
};

class ATCDownlinkMsgElementId : public Asn1::Choice {
public:
  typedef ATCDownlinkMsgElementId PrimitiveType;
  enum {
    dM0NULL_idx = 0,
    dM1NULL_idx = 1,
    dM2NULL_idx = 2,
    dM3NULL_idx = 3,
    dM4NULL_idx = 4,
    dM5NULL_idx = 5,
    dM6Level_idx = 6,
    dM7LevelLevel_idx = 7,
    dM8Level_idx = 8,
    dM9Level_idx = 9,
    dM10Level_idx = 10,
    dM11PositionLevel_idx = 11,
    dM12PositionLevel_idx = 12,
    dM13TimeLevel_idx = 13,
    dM14TimeLevel_idx = 14,
    dM15DistanceSpecifiedDirection_idx = 15,
    dM16PositionDistanceSpecifiedDirection_idx = 16,
    dM17TimeDistanceSpecifiedDirection_idx = 17,
    dM18Speed_idx = 18,
    dM19SpeedSpeed_idx = 19,
    dM20NULL_idx = 20,
    dM21Frequency_idx = 21,
    dM22Position_idx = 22,
    dM23ProcedureName_idx = 23,
    dM24RouteClearance_idx = 24,
    dM25ClearanceType_idx = 25,
    dM26PositionRouteClearance_idx = 26,
    dM27DistanceSpecifiedDirection_idx = 27,
    dM28Level_idx = 28,
    dM29Level_idx = 29,
    dM30Level_idx = 30,
    dM31Position_idx = 31,
    dM32Level_idx = 32,
    dM33Position_idx = 33,
    dM34Speed_idx = 34,
    dM35Degrees_idx = 35,
    dM36Degrees_idx = 36,
    dM37Level_idx = 37,
    dM38Level_idx = 38,
    dM39Speed_idx = 39,
    dM40RouteClearance_idx = 40,
    dM41NULL_idx = 41,
    dM42Position_idx = 42,
    dM43Time_idx = 43,
    dM44Position_idx = 44,
    dM45Position_idx = 45,
    dM46Time_idx = 46,
    dM47Code_idx = 47,
    dM48PositionReport_idx = 48,
    dM49Speed_idx = 49,
    dM50SpeedSpeed_idx = 50,
    dM51NULL_idx = 51,
    dM52NULL_idx = 52,
    dM53NULL_idx = 53,
    dM54Level_idx = 54,
    dM55NULL_idx = 55,
    dM56NULL_idx = 56,
    dM57RemainingFuelPersonsOnBoard_idx = 57,
    dM58NULL_idx = 58,
    dM59PositionRouteClearance_idx = 59,
    dM60DistanceSpecifiedDirection_idx = 60,
    dM61Level_idx = 61,
    dM62ErrorInformation_idx = 62,
    dM63NULL_idx = 63,
    dM64FacilityDesignation_idx = 64,
    dM65NULL_idx = 65,
    dM66NULL_idx = 66,
    dM67FreeText_idx = 67,
    dM68FreeText_idx = 68,
    dM69NULL_idx = 69,
    dM70Degrees_idx = 70,
    dM71Degrees_idx = 71,
    dM72Level_idx = 72,
    dM73Versionnumber_idx = 73,
    dM74NULL_idx = 74,
    dM75NULL_idx = 75,
    dM76LevelLevel_idx = 76,
    dM77LevelLevel_idx = 77,
    dM78TimeDistanceToFromPosition_idx = 78,
    dM79AtisCode_idx = 79,
    dM80DistanceSpecifiedDirection_idx = 80,
    dM81LevelTime_idx = 81,
    dM82Level_idx = 82,
    dM83SpeedTime_idx = 83,
    dM84Speed_idx = 84,
    dM85DistanceSpecifiedDirectionTime_idx = 85,
    dM86DistanceSpecifiedDirection_idx = 86,
    dM87Level_idx = 87,
    dM88Level_idx = 88,
    dM89UnitnameFrequency_idx = 89,
    dM90FreeText_idx = 90,
    dM91FreeText_idx = 91,
    dM92FreeText_idx = 92,
    dM93FreeText_idx = 93,
    dM94FreeText_idx = 94,
    dM95FreeText_idx = 95,
    dM96FreeText_idx = 96,
    dM97FreeText_idx = 97,
    dM98FreeText_idx = 98,
    dM99NULL_idx = 99,
    dM100NULL_idx = 100,
    dM101NULL_idx = 101,
    dM102NULL_idx = 102,
    dM103NULL_idx = 103,
    dM104PositionTime_idx = 104,
    dM105Airport_idx = 105,
    dM106Level_idx = 106,
    dM107NULL_idx = 107,
    dM108NULL_idx = 108,
    dM109Time_idx = 109,
    dM110Position_idx = 110,
    dM111TimePosition_idx = 111,
    dM112NULL_idx = 112,
    dM113SpeedTypeSpeedTypeSpeedTypeSpeed_idx = 113
  };
  virtual bool extensible() const { return true; }
  Option<dM0NULL_idx, Asn1::Asn1NULL>::type dM0NULL();
  Option<dM1NULL_idx, Asn1::Asn1NULL>::type dM1NULL();
  Option<dM2NULL_idx, Asn1::Asn1NULL>::type dM2NULL();
  Option<dM3NULL_idx, Asn1::Asn1NULL>::type dM3NULL();
  Option<dM4NULL_idx, Asn1::Asn1NULL>::type dM4NULL();
  Option<dM5NULL_idx, Asn1::Asn1NULL>::type dM5NULL();
  Option<dM6Level_idx, Level>::type dM6Level();
  Option<dM7LevelLevel_idx, LevelLevel>::type dM7LevelLevel();
  Option<dM8Level_idx, Level>::type dM8Level();
  Option<dM9Level_idx, Level>::type dM9Level();
  Option<dM10Level_idx, Level>::type dM10Level();
  Option<dM11PositionLevel_idx, PositionLevel>::type dM11PositionLevel();
  Option<dM12PositionLevel_idx, PositionLevel>::type dM12PositionLevel();
  Option<dM13TimeLevel_idx, TimeLevel>::type dM13TimeLevel();
  Option<dM14TimeLevel_idx, TimeLevel>::type dM14TimeLevel();
  Option<dM15DistanceSpecifiedDirection_idx, DistanceSpecifiedDirection>::type dM15DistanceSpecifiedDirection();
  Option<dM16PositionDistanceSpecifiedDirection_idx, PositionDistanceSpecifiedDirection>::type dM16PositionDistanceSpecifiedDirection();
  Option<dM17TimeDistanceSpecifiedDirection_idx, TimeDistanceSpecifiedDirection>::type dM17TimeDistanceSpecifiedDirection();
  Option<dM18Speed_idx, Speed>::type dM18Speed();
  Option<dM19SpeedSpeed_idx, SpeedSpeed>::type dM19SpeedSpeed();
  Option<dM20NULL_idx, Asn1::Asn1NULL>::type dM20NULL();
  Option<dM21Frequency_idx, Frequency>::type dM21Frequency();
  Option<dM22Position_idx, Position>::type dM22Position();
  Option<dM23ProcedureName_idx, ProcedureName>::type dM23ProcedureName();
  Option<dM24RouteClearance_idx, RouteClearanceIndex>::type dM24RouteClearance();
  Option<dM25ClearanceType_idx, ClearanceType>::type dM25ClearanceType();
  Option<dM26PositionRouteClearance_idx, PositionRouteClearanceIndex>::type dM26PositionRouteClearance();
  Option<dM27DistanceSpecifiedDirection_idx, DistanceSpecifiedDirection>::type dM27DistanceSpecifiedDirection();
  Option<dM28Level_idx, Level>::type dM28Level();
  Option<dM29Level_idx, Level>::type dM29Level();
  Option<dM30Level_idx, Level>::type dM30Level();
  Option<dM31Position_idx, Position>::type dM31Position();
  Option<dM32Level_idx, Level>::type dM32Level();
  Option<dM33Position_idx, Position>::type dM33Position();
  Option<dM34Speed_idx, Speed>::type dM34Speed();
  Option<dM35Degrees_idx, Degrees>::type dM35Degrees();
  Option<dM36Degrees_idx, Degrees>::type dM36Degrees();
  Option<dM37Level_idx, Level>::type dM37Level();
  Option<dM38Level_idx, Level>::type dM38Level();
  Option<dM39Speed_idx, Speed>::type dM39Speed();
  Option<dM40RouteClearance_idx, RouteClearanceIndex>::type dM40RouteClearance();
  Option<dM41NULL_idx, Asn1::Asn1NULL>::type dM41NULL();
  Option<dM42Position_idx, Position>::type dM42Position();
  Option<dM43Time_idx, Time>::type dM43Time();
  Option<dM44Position_idx, Position>::type dM44Position();
  Option<dM45Position_idx, Position>::type dM45Position();
  Option<dM46Time_idx, Time>::type dM46Time();
  Option<dM47Code_idx, Code>::type dM47Code();
  Option<dM48PositionReport_idx, PositionReport>::type dM48PositionReport();
  Option<dM49Speed_idx, Speed>::type dM49Speed();
  Option<dM50SpeedSpeed_idx, SpeedSpeed>::type dM50SpeedSpeed();
  Option<dM51NULL_idx, Asn1::Asn1NULL>::type dM51NULL();
  Option<dM52NULL_idx, Asn1::Asn1NULL>::type dM52NULL();
  Option<dM53NULL_idx, Asn1::Asn1NULL>::type dM53NULL();
  Option<dM54Level_idx, Level>::type dM54Level();
  Option<dM55NULL_idx, Asn1::Asn1NULL>::type dM55NULL();
  Option<dM56NULL_idx, Asn1::Asn1NULL>::type dM56NULL();
  Option<dM57RemainingFuelPersonsOnBoard_idx, RemainingFuelPersonsOnBoard>::type dM57RemainingFuelPersonsOnBoard();
  Option<dM58NULL_idx, Asn1::Asn1NULL>::type dM58NULL();
  Option<dM59PositionRouteClearance_idx, PositionRouteClearanceIndex>::type dM59PositionRouteClearance();
  Option<dM60DistanceSpecifiedDirection_idx, DistanceSpecifiedDirection>::type dM60DistanceSpecifiedDirection();
  Option<dM61Level_idx, Level>::type dM61Level();
  Option<dM62ErrorInformation_idx, ErrorInformation>::type dM62ErrorInformation();
  Option<dM63NULL_idx, Asn1::Asn1NULL>::type dM63NULL();
  Option<dM64FacilityDesignation_idx, FacilityDesignation>::type dM64FacilityDesignation();
  Option<dM65NULL_idx, Asn1::Asn1NULL>::type dM65NULL();
  Option<dM66NULL_idx, Asn1::Asn1NULL>::type dM66NULL();
  Option<dM67FreeText_idx, FreeText>::type dM67FreeText();
  Option<dM68FreeText_idx, FreeText>::type dM68FreeText();
  Option<dM69NULL_idx, Asn1::Asn1NULL>::type dM69NULL();
  Option<dM70Degrees_idx, Degrees>::type dM70Degrees();
  Option<dM71Degrees_idx, Degrees>::type dM71Degrees();
  Option<dM72Level_idx, Level>::type dM72Level();
  Option<dM73Versionnumber_idx, VersionNumber>::type dM73Versionnumber();
  Option<dM74NULL_idx, Asn1::Asn1NULL>::type dM74NULL();
  Option<dM75NULL_idx, Asn1::Asn1NULL>::type dM75NULL();
  Option<dM76LevelLevel_idx, LevelLevel>::type dM76LevelLevel();
  Option<dM77LevelLevel_idx, LevelLevel>::type dM77LevelLevel();
  Option<dM78TimeDistanceToFromPosition_idx, TimeDistanceToFromPosition>::type dM78TimeDistanceToFromPosition();
  Option<dM79AtisCode_idx, ATISCode>::type dM79AtisCode();
  Option<dM80DistanceSpecifiedDirection_idx, DistanceSpecifiedDirection>::type dM80DistanceSpecifiedDirection();
  Option<dM81LevelTime_idx, LevelTime>::type dM81LevelTime();
  Option<dM82Level_idx, Level>::type dM82Level();
  Option<dM83SpeedTime_idx, SpeedTime>::type dM83SpeedTime();
  Option<dM84Speed_idx, Speed>::type dM84Speed();
  Option<dM85DistanceSpecifiedDirectionTime_idx, DistanceSpecifiedDirectionTime>::type dM85DistanceSpecifiedDirectionTime();
  Option<dM86DistanceSpecifiedDirection_idx, DistanceSpecifiedDirection>::type dM86DistanceSpecifiedDirection();
  Option<dM87Level_idx, Level>::type dM87Level();
  Option<dM88Level_idx, Level>::type dM88Level();
  Option<dM89UnitnameFrequency_idx, UnitNameFrequency>::type dM89UnitnameFrequency();
  Option<dM90FreeText_idx, FreeText>::type dM90FreeText();
  Option<dM91FreeText_idx, FreeText>::type dM91FreeText();
  Option<dM92FreeText_idx, FreeText>::type dM92FreeText();
  Option<dM93FreeText_idx, FreeText>::type dM93FreeText();
  Option<dM94FreeText_idx, FreeText>::type dM94FreeText();
  Option<dM95FreeText_idx, FreeText>::type dM95FreeText();
  Option<dM96FreeText_idx, FreeText>::type dM96FreeText();
  Option<dM97FreeText_idx, FreeText>::type dM97FreeText();
  Option<dM98FreeText_idx, FreeText>::type dM98FreeText();
  Option<dM99NULL_idx, Asn1::Asn1NULL>::type dM99NULL();
  Option<dM100NULL_idx, Asn1::Asn1NULL>::type dM100NULL();
  Option<dM101NULL_idx, Asn1::Asn1NULL>::type dM101NULL();
  Option<dM102NULL_idx, Asn1::Asn1NULL>::type dM102NULL();
  Option<dM103NULL_idx, Asn1::Asn1NULL>::type dM103NULL();
  Option<dM104PositionTime_idx, PositionTime>::type dM104PositionTime();
  Option<dM105Airport_idx, Airport>::type dM105Airport();
  Option<dM106Level_idx, Level>::type dM106Level();
  Option<dM107NULL_idx, Asn1::Asn1NULL>::type dM107NULL();
  Option<dM108NULL_idx, Asn1::Asn1NULL>::type dM108NULL();
  Option<dM109Time_idx, Time>::type dM109Time();
  Option<dM110Position_idx, Position>::type dM110Position();
  Option<dM111TimePosition_idx, TimePosition>::type dM111TimePosition();
  Option<dM112NULL_idx, Asn1::Asn1NULL>::type dM112NULL();
  Option<dM113SpeedTypeSpeedTypeSpeedTypeSpeed_idx, SpeedTypeSpeedTypeSpeedTypeSpeed>::type dM113SpeedTypeSpeedTypeSpeedTypeSpeed();
  ATCDownlinkMsgElementId() : Choice() { }
};

class Altimeter : public Asn1::Choice {
public:
  typedef Altimeter PrimitiveType;
  enum {
    altimeterEnglish_idx = 0,
    altimeterMetric_idx = 1
  };
  virtual bool extensible() const { return false; }
  Option<altimeterEnglish_idx, AltimeterEnglish>::type altimeterEnglish();
  Option<altimeterMetric_idx, AltimeterMetric>::type altimeterMetric();
  Altimeter() : Choice() { }
};

class ATWAlongTrackWaypoint : public Asn1::Sequence {
public:
  Required<Position>::type position;
  Required<ATWDistance>::type aTWDistance;
  Optional<Speed>::type speed;
  Optional<ATWLevelSequence>::type aTWLevels;
  ATWAlongTrackWaypoint() : Sequence(4) {
    Sequence::Inserter it = back_inserter();
    it << position << aTWDistance << speed << aTWLevels;
  }
};

class ATWLevel : public Asn1::Sequence {
public:
  Required<ATWLevelTolerance>::type atw;
  Required<Level>::type level;
  ATWLevel() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << atw << level;
  }
};

class ATWLevelTolerance : public Asn1::Enumerated<0,2> {
public:
  virtual bool extensible() const { return false; }
  typedef enum {
    at = 0,
    atorabove = 1,
    atorbelow = 2
  } EnumValues;
  ATWLevelTolerance() : Enumerated() { }
  ATWLevelTolerance(int value) : Enumerated(value) { }
};

class ATWDistance : public Asn1::Sequence {
public:
  Required<ATWDistanceTolerance>::type atwDistanceTolerance;
  Required<Distance>::type distance;
  ATWDistance() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << atwDistanceTolerance << distance;
  }
};

class ATWDistanceTolerance : public Asn1::Enumerated<0,1> {
public:
  virtual bool extensible() const { return false; }
  typedef enum {
    plus = 0,
    minus = 1
  } EnumValues;
  ATWDistanceTolerance() : Enumerated() { }
  ATWDistanceTolerance(int value) : Enumerated(value) { }
};

class ClearanceType : public Asn1::Enumerated<0,11> {
public:
  virtual bool extensible() const { return true; }
  typedef enum {
    noneSpecified = 0,
    approach = 1,
    departure = 2,
    further = 3,
    start_up = 4,
    pushback = 5,
    taxi = 6,
    take_off = 7,
    landing = 8,
    oceanic = 9,
    en_route = 10,
    downstream = 11
  } EnumValues;
  ClearanceType() : Enumerated() { }
  ClearanceType(int value) : Enumerated(value) { }
};

class ControlledTime : public Asn1::Sequence {
public:
  Required<Time>::type time;
  Required<TimeTolerance>::type timeTolerance;
  ControlledTime() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << time << timeTolerance;
  }
};

class Date : public Asn1::Sequence {
public:
  Required<Year>::type year;
  Required<Month>::type month;
  Required<Day>::type day;
  Date() : Sequence(3) {
    Sequence::Inserter it = back_inserter();
    it << year << month << day;
  }
};

class DateTimeGroup : public Asn1::Sequence {
public:
  Required<Date>::type date;
  Required<Timehhmmss>::type timehhmmss;
  DateTimeGroup() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << date << timehhmmss;
  }
};

class Degrees : public Asn1::Choice {
public:
  typedef Degrees PrimitiveType;
  enum {
    degreesMagnetic_idx = 0,
    degreesTrue_idx = 1
  };
  virtual bool extensible() const { return false; }
  Option<degreesMagnetic_idx, DegreesMagnetic>::type degreesMagnetic();
  Option<degreesTrue_idx, DegreesTrue>::type degreesTrue();
  Degrees() : Choice() { }
};

class DepartureClearance : public Asn1::Sequence {
public:
  Required<AircraftFlightIdentification>::type aircraftFlightIdentification;
  Required<Position>::type clearanceLimit;
  Optional<FlightInformation>::type flightInformation;
  Optional<FurtherInstructions>::type furtherInstructions;
  DepartureClearance() : Sequence(4) {
    Sequence::Inserter it = back_inserter();
    it << aircraftFlightIdentification << clearanceLimit << flightInformation << furtherInstructions;
  }
};

class Direction : public Asn1::Enumerated<0,10> {
public:
  virtual bool extensible() const { return false; }
  typedef enum {
    left = 0,
    right = 1,
    eitherSide = 2,
    north = 3,
    south = 4,
    east = 5,
    west = 6,
    northEast = 7,
    northWest = 8,
    southEast = 9,
    southWest = 10
  } EnumValues;
  Direction() : Enumerated() { }
  Direction(int value) : Enumerated(value) { }
};

class DirectionDegrees : public Asn1::Sequence {
public:
  Required<Direction>::type direction;
  Required<Degrees>::type degrees;
  DirectionDegrees() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << direction << degrees;
  }
};

class Distance : public Asn1::Choice {
public:
  typedef Distance PrimitiveType;
  enum {
    distanceNm_idx = 0,
    distanceKm_idx = 1
  };
  virtual bool extensible() const { return false; }
  Option<distanceNm_idx, DistanceNm>::type distanceNm();
  Option<distanceKm_idx, DistanceKm>::type distanceKm();
  Distance() : Choice() { }
};

class DistanceSpecified : public Asn1::Choice {
public:
  typedef DistanceSpecified PrimitiveType;
  enum {
    distanceSpecifiedNm_idx = 0,
    distanceSpecifiedKm_idx = 1
  };
  virtual bool extensible() const { return false; }
  Option<distanceSpecifiedNm_idx, DistanceSpecifiedNm>::type distanceSpecifiedNm();
  Option<distanceSpecifiedKm_idx, DistanceSpecifiedKm>::type distanceSpecifiedKm();
  DistanceSpecified() : Choice() { }
};

class DistanceSpecifiedDirection : public Asn1::Sequence {
public:
  Required<DistanceSpecified>::type distanceSpecified;
  Required<Direction>::type direction;
  DistanceSpecifiedDirection() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << distanceSpecified << direction;
  }
};

class DistanceSpecifiedDirectionTime : public Asn1::Sequence {
public:
  Required<DistanceSpecifiedDirection>::type distanceSpecifiedDirection;
  Required<Time>::type time;
  DistanceSpecifiedDirectionTime() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << distanceSpecifiedDirection << time;
  }
};

class ErrorInformation : public Asn1::Enumerated<0,4> {
public:
  virtual bool extensible() const { return true; }
  typedef enum {
    unrecognizedMsgReferenceNumber = 0,
    logicalAcknowledgmentNotAccepted = 1,
    insufficientResources = 2,
    invalidMessageElementCombination = 3,
    invalidMessageElement = 4
  } EnumValues;
  ErrorInformation() : Enumerated() { }
  ErrorInformation(int value) : Enumerated(value) { }
};

class Facility : public Asn1::Choice {
public:
  typedef Facility PrimitiveType;
  enum {
    noFacility_idx = 0,
    facilityDesignation_idx = 1
  };
  virtual bool extensible() const { return false; }
  Option<noFacility_idx, Asn1::Asn1NULL>::type noFacility();
  Option<facilityDesignation_idx, FacilityDesignation>::type facilityDesignation();
  Facility() : Choice() { }
};

class FacilityFunction : public Asn1::Enumerated<0,8> {
public:
  virtual bool extensible() const { return true; }
  typedef enum {
    center = 0,
    approach = 1,
    tower = 2,
    final = 3,
    groundControl = 4,
    clearanceDelivery = 5,
    departure = 6,
    control = 7,
    radio = 8
  } EnumValues;
  FacilityFunction() : Enumerated() { }
  FacilityFunction(int value) : Enumerated(value) { }
};

class FacilityDesignationAltimeter : public Asn1::Sequence {
public:
  Required<FacilityDesignation>::type facilityDesignation;
  Required<Altimeter>::type altimeter;
  FacilityDesignationAltimeter() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << facilityDesignation << altimeter;
  }
};

class FacilityDesignationATISCode : public Asn1::Sequence {
public:
  Required<FacilityDesignation>::type facilityDesignation;
  Required<ATISCode>::type aTISCode;
  FacilityDesignationATISCode() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << facilityDesignation << aTISCode;
  }
};

class FixName : public Asn1::Sequence {
public:
  Required<Fix>::type name;
  Optional<LatitudeLongitude>::type latlon;
  FixName() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << name << latlon;
  }
};

class FlightInformation : public Asn1::Choice {
public:
  typedef FlightInformation PrimitiveType;
  enum {
    routeOfFlight_idx = 0,
    levelsOfFlight_idx = 1,
    routeAndLevels_idx = 2
  };
  virtual bool extensible() const { return false; }
  Option<routeOfFlight_idx, RouteInformation>::type routeOfFlight();
  Option<levelsOfFlight_idx, LevelsOfFlight>::type levelsOfFlight();
  Option<routeAndLevels_idx, RouteAndLevels>::type routeAndLevels();
  FlightInformation() : Choice() { }
};

class Frequency : public Asn1::Choice {
public:
  typedef Frequency PrimitiveType;
  enum {
    frequencyhf_idx = 0,
    frequencyvhf_idx = 1,
    frequencyuhf_idx = 2,
    frequencysatchannel_idx = 3
  };
  virtual bool extensible() const { return false; }
  Option<frequencyhf_idx, Frequencyhf>::type frequencyhf();
  Option<frequencyvhf_idx, Frequencyvhf>::type frequencyvhf();
  Option<frequencyuhf_idx, Frequencyuhf>::type frequencyuhf();
  Option<frequencysatchannel_idx, Frequencysatchannel>::type frequencysatchannel();
  Frequency() : Choice() { }
};

class FurtherInstructions : public Asn1::Sequence {
public:
  Optional<Code>::type code;
  Optional<UnitNameFrequency>::type frequencyDeparture;
  Optional<Time>::type clearanceExpiryTime;
  Optional<Airport>::type airportDeparture;
  Optional<Airport>::type airportDestination;
  Optional<TimeDeparture>::type timeDeparture;
  Optional<Runway>::type runwayDeparture;
  Optional<RevisionNumber>::type revisionNumber;
  Optional<ATISCode>::type aTISCode;
  FurtherInstructions() : Sequence(9) {
    Sequence::Inserter it = back_inserter();
    it << code << frequencyDeparture << clearanceExpiryTime << airportDeparture << airportDestination << timeDeparture << runwayDeparture << revisionNumber << aTISCode;
  }
};

class Holdatwaypoint : public Asn1::Sequence {
public:
  Required<Position>::type position;
  Optional<Speed>::type holdatwaypointspeedlow;
  Optional<ATWLevel>::type aTWlevel;
  Optional<Speed>::type holdatwaypointspeedhigh;
  Optional<Direction>::type direction;
  Optional<Degrees>::type degrees;
  Optional<Time>::type eFCtime;
  Optional<LegType>::type legtype;
  Holdatwaypoint() : Sequence(8) {
    Sequence::Inserter it = back_inserter();
    it << position << holdatwaypointspeedlow << aTWlevel << holdatwaypointspeedhigh << direction << degrees << eFCtime << legtype;
  }
};

class HoldClearance : public Asn1::Sequence {
public:
  Required<Position>::type position;
  Required<Level>::type level;
  Required<Degrees>::type degrees;
  Required<Direction>::type direction;
  Optional<LegType>::type legType;
  HoldClearance() : Sequence(5) {
    Sequence::Inserter it = back_inserter();
    it << position << level << degrees << direction << legType;
  }
};

class InterceptCourseFrom : public Asn1::Sequence {
public:
  Required<InterceptCourseFromSelection>::type fromSelection;
  Required<Degrees>::type degrees;
  InterceptCourseFrom() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << fromSelection << degrees;
  }
};

class InterceptCourseFromSelection : public Asn1::Choice {
public:
  typedef InterceptCourseFromSelection PrimitiveType;
  enum {
    publishedIdentifier_idx = 0,
    latitudeLongitude_idx = 1,
    placeBearingPlaceBearing_idx = 2,
    placeBearingDistance_idx = 3
  };
  virtual bool extensible() const { return false; }
  Option<publishedIdentifier_idx, PublishedIdentifier>::type publishedIdentifier();
  Option<latitudeLongitude_idx, LatitudeLongitude>::type latitudeLongitude();
  Option<placeBearingPlaceBearing_idx, PlaceBearingPlaceBearing>::type placeBearingPlaceBearing();
  Option<placeBearingDistance_idx, PlaceBearingDistance>::type placeBearingDistance();
  InterceptCourseFromSelection() : Choice() { }
};

class Icing : public Asn1::Enumerated<0,3> {
public:
  virtual bool extensible() const { return false; }
  typedef enum {
    reserved = 0,
    light = 1,
    moderate = 2,
    severe = 3
  } EnumValues;
  Icing() : Enumerated() { }
  Icing(int value) : Enumerated(value) { }
};

class Latitude : public Asn1::Sequence {
public:
  Required<LatitudeType>::type latitudeType;
  Required<LatitudeDirection>::type latitudeDirection;
  Latitude() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << latitudeType << latitudeDirection;
  }
};

class LatitudeDegreesMinutes : public Asn1::Sequence {
public:
  Required<LatitudeWholeDegrees>::type latitudeWholeDegrees;
  Required<MinutesLatLon>::type minutesLatLon;
  LatitudeDegreesMinutes() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << latitudeWholeDegrees << minutesLatLon;
  }
};

class LatitudeDegreesMinutesSeconds : public Asn1::Sequence {
public:
  Required<LatitudeWholeDegrees>::type latitudeWholeDegrees;
  Required<LatLonWholeMinutes>::type latlonWholeMinutes;
  Required<SecondsLatLon>::type secondsLatLon;
  LatitudeDegreesMinutesSeconds() : Sequence(3) {
    Sequence::Inserter it = back_inserter();
    it << latitudeWholeDegrees << latlonWholeMinutes << secondsLatLon;
  }
};

class LatitudeDirection : public Asn1::Enumerated<0,1> {
public:
  virtual bool extensible() const { return false; }
  typedef enum {
    north = 0,
    south = 1
  } EnumValues;
  LatitudeDirection() : Enumerated() { }
  LatitudeDirection(int value) : Enumerated(value) { }
};

class LatitudeLongitude : public Asn1::Sequence {
public:
  Optional<Latitude>::type latitude;
  Optional<Longitude>::type longitude;
  LatitudeLongitude() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << latitude << longitude;
  }
};

class LatitudeReportingPoints : public Asn1::Sequence {
public:
  Required<LatitudeDirection>::type latitudeDirection;
  Required<LatitudeDegrees>::type latitudeDegrees;
  LatitudeReportingPoints() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << latitudeDirection << latitudeDegrees;
  }
};

class LatitudeType : public Asn1::Choice {
public:
  typedef LatitudeType PrimitiveType;
  enum {
    latitudeDegrees_idx = 0,
    latitudeDegreesMinutes_idx = 1,
    latitudeDMS_idx = 2
  };
  virtual bool extensible() const { return false; }
  Option<latitudeDegrees_idx, LatitudeDegrees>::type latitudeDegrees();
  Option<latitudeDegreesMinutes_idx, LatitudeDegreesMinutes>::type latitudeDegreesMinutes();
  Option<latitudeDMS_idx, LatitudeDegreesMinutesSeconds>::type latitudeDMS();
  LatitudeType() : Choice() { }
};

class LatLonReportingPoints : public Asn1::Choice {
public:
  typedef LatLonReportingPoints PrimitiveType;
  enum {
    latitudeReportingPoints_idx = 0,
    longitudeReportingPoints_idx = 1
  };
  virtual bool extensible() const { return false; }
  Option<latitudeReportingPoints_idx, LatitudeReportingPoints>::type latitudeReportingPoints();
  Option<longitudeReportingPoints_idx, LongitudeReportingPoints>::type longitudeReportingPoints();
  LatLonReportingPoints() : Choice() { }
};

class LegDistance : public Asn1::Choice {
public:
  typedef LegDistance PrimitiveType;
  enum {
    legDistanceEnglish_idx = 0,
    legDistanceMetric_idx = 1
  };
  virtual bool extensible() const { return false; }
  Option<legDistanceEnglish_idx, LegDistanceEnglish>::type legDistanceEnglish();
  Option<legDistanceMetric_idx, LegDistanceMetric>::type legDistanceMetric();
  LegDistance() : Choice() { }
};

class LegType : public Asn1::Choice {
public:
  typedef LegType PrimitiveType;
  enum {
    legDistance_idx = 0,
    legTime_idx = 1
  };
  virtual bool extensible() const { return false; }
  Option<legDistance_idx, LegDistance>::type legDistance();
  Option<legTime_idx, LegTime>::type legTime();
  LegType() : Choice() { }
};

class Level : public Asn1::Choice {
public:
  typedef Level PrimitiveType;
  enum {
    singleLevel_idx = 0,
    blockLevel_idx = 1
  };
  virtual bool extensible() const { return false; }
  Option<singleLevel_idx, LevelType>::type singleLevel();
  Option<blockLevel_idx, Asn1::SequenceOf<LevelType, 2, 2> >::type blockLevel();
  Level() : Choice() { }
};

class LevelPosition : public Asn1::Sequence {
public:
  Required<Level>::type level;
  Required<Position>::type position;
  LevelPosition() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << level << position;
  }
};

class LevelProcedureName : public Asn1::Sequence {
public:
  Required<Level>::type level;
  Required<ProcedureName>::type procedureName;
  LevelProcedureName() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << level << procedureName;
  }
};

class LevelsOfFlight : public Asn1::Choice {
public:
  typedef LevelsOfFlight PrimitiveType;
  enum {
    level_idx = 0,
    procedureName_idx = 1,
    levelProcedureName_idx = 2
  };
  virtual bool extensible() const { return false; }
  Option<level_idx, Level>::type level();
  Option<procedureName_idx, ProcedureName>::type procedureName();
  Option<levelProcedureName_idx, LevelProcedureName>::type levelProcedureName();
  LevelsOfFlight() : Choice() { }
};

class LevelSpeed : public Asn1::Sequence {
public:
  Required<Level>::type level;
  Required<SpeedSpeed>::type speed;
  LevelSpeed() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << level << speed;
  }
};

class LevelSpeedSpeed : public Asn1::Sequence {
public:
  Required<Level>::type level;
  Required<SpeedSpeed>::type speeds;
  LevelSpeedSpeed() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << level << speeds;
  }
};

class LevelTime : public Asn1::Sequence {
public:
  Required<Level>::type level;
  Required<Time>::type time;
  LevelTime() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << level << time;
  }
};

class LevelType : public Asn1::Choice {
public:
  typedef LevelType PrimitiveType;
  enum {
    levelFeet_idx = 0,
    levelMeters_idx = 1,
    levelFlightLevel_idx = 2,
    levelFlightLevelMetric_idx = 3
  };
  virtual bool extensible() const { return false; }
  Option<levelFeet_idx, LevelFeet>::type levelFeet();
  Option<levelMeters_idx, LevelMeters>::type levelMeters();
  Option<levelFlightLevel_idx, LevelFlightLevel>::type levelFlightLevel();
  Option<levelFlightLevelMetric_idx, LevelFlightLevelMetric>::type levelFlightLevelMetric();
  LevelType() : Choice() { }
};

class Longitude : public Asn1::Sequence {
public:
  Required<LongitudeType>::type longitudeType;
  Required<LongitudeDirection>::type longitudeDirection;
  Longitude() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << longitudeType << longitudeDirection;
  }
};

class LongitudeDegreesMinutes : public Asn1::Sequence {
public:
  Required<LongitudeWholeDegrees>::type longitudeWholeDegrees;
  Required<MinutesLatLon>::type minutesLatLon;
  LongitudeDegreesMinutes() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << longitudeWholeDegrees << minutesLatLon;
  }
};

class LongitudeDegreesMinutesSeconds : public Asn1::Sequence {
public:
  Required<LongitudeWholeDegrees>::type longitudeWholeDegrees;
  Required<LatLonWholeMinutes>::type latLonWholeMinutes;
  Required<SecondsLatLon>::type secondsLatLon;
  LongitudeDegreesMinutesSeconds() : Sequence(3) {
    Sequence::Inserter it = back_inserter();
    it << longitudeWholeDegrees << latLonWholeMinutes << secondsLatLon;
  }
};

class LongitudeDirection : public Asn1::Enumerated<0,1> {
public:
  virtual bool extensible() const { return false; }
  typedef enum {
    east = 0,
    west = 1
  } EnumValues;
  LongitudeDirection() : Enumerated() { }
  LongitudeDirection(int value) : Enumerated(value) { }
};

class LongitudeReportingPoints : public Asn1::Sequence {
public:
  Required<LongitudeDirection>::type longitudeDirection;
  Required<LongitudeDegrees>::type longitudeDegrees;
  LongitudeReportingPoints() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << longitudeDirection << longitudeDegrees;
  }
};

class LongitudeType : public Asn1::Choice {
public:
  typedef LongitudeType PrimitiveType;
  enum {
    longitudeDegrees_idx = 0,
    longitudeDegreesMinutes_idx = 1,
    longitudeDMS_idx = 2
  };
  virtual bool extensible() const { return false; }
  Option<longitudeDegrees_idx, LongitudeDegrees>::type longitudeDegrees();
  Option<longitudeDegreesMinutes_idx, LongitudeDegreesMinutes>::type longitudeDegreesMinutes();
  Option<longitudeDMS_idx, LongitudeDegreesMinutesSeconds>::type longitudeDMS();
  LongitudeType() : Choice() { }
};

class Navaid : public Asn1::Sequence {
public:
  Required<NavaidName>::type name;
  Optional<LatitudeLongitude>::type latlon;
  Navaid() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << name << latlon;
  }
};

class PlaceBearing : public Asn1::Sequence {
public:
  Required<PublishedIdentifier>::type publishedIdentifier;
  Required<Degrees>::type degrees;
  PlaceBearing() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << publishedIdentifier << degrees;
  }
};

class PlaceBearingDistance : public Asn1::Sequence {
public:
  Required<PublishedIdentifier>::type publishedIdentifier;
  Required<Degrees>::type degrees;
  Required<Distance>::type distance;
  PlaceBearingDistance() : Sequence(3) {
    Sequence::Inserter it = back_inserter();
    it << publishedIdentifier << degrees << distance;
  }
};

class Position : public Asn1::Choice {
public:
  typedef Position PrimitiveType;
  enum {
    fixName_idx = 0,
    navaid_idx = 1,
    airport_idx = 2,
    latitudeLongitude_idx = 3,
    placeBearingDistance_idx = 4
  };
  virtual bool extensible() const { return false; }
  Option<fixName_idx, FixName>::type fixName();
  Option<navaid_idx, Navaid>::type navaid();
  Option<airport_idx, Airport>::type airport();
  Option<latitudeLongitude_idx, LatitudeLongitude>::type latitudeLongitude();
  Option<placeBearingDistance_idx, PlaceBearingDistance>::type placeBearingDistance();
  Position() : Choice() { }
};

class PositionDegrees : public Asn1::Sequence {
public:
  Required<Position>::type position;
  Required<Degrees>::type degrees;
  PositionDegrees() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << position << degrees;
  }
};

class PositionDistanceSpecifiedDirection : public Asn1::Sequence {
public:
  Required<Position>::type position;
  Required<DistanceSpecifiedDirection>::type distanceSpecifiedDirection;
  PositionDistanceSpecifiedDirection() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << position << distanceSpecifiedDirection;
  }
};

class PositionLevel : public Asn1::Sequence {
public:
  Required<Position>::type position;
  Required<Level>::type level;
  PositionLevel() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << position << level;
  }
};

class PositionLevelLevel : public Asn1::Sequence {
public:
  Required<Position>::type position;
  Required<LevelLevel>::type levels;
  PositionLevelLevel() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << position << levels;
  }
};

class PositionLevelSpeed : public Asn1::Sequence {
public:
  Required<PositionLevel>::type positionlevel;
  Required<Speed>::type speed;
  PositionLevelSpeed() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << positionlevel << speed;
  }
};

class PositionProcedureName : public Asn1::Sequence {
public:
  Required<Position>::type position;
  Required<ProcedureName>::type procedureName;
  PositionProcedureName() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << position << procedureName;
  }
};

class PositionReport : public Asn1::Sequence {
public:
  Required<Position>::type positioncurrent;
  Required<Time>::type timeatpositioncurrent;
  Required<Level>::type level;
  Optional<Position>::type fixnext;
  Optional<Time>::type timeetaatfixnext;
  Optional<Position>::type fixnextplusone;
  Optional<Time>::type timeetaatdestination;
  Optional<RemainingFuel>::type remainingFuel;
  Optional<Temperature>::type temperature;
  Optional<Winds>::type winds;
  Optional<Turbulence>::type turbulence;
  Optional<Icing>::type icing;
  Optional<Speed>::type speed;
  Optional<SpeedGround>::type speedground;
  Optional<VerticalChange>::type verticalChange;
  Optional<Degrees>::type trackAngle;
  Optional<Degrees>::type heading;
  Optional<Distance>::type distance;
  Optional<Humidity>::type humidity;
  Optional<Position>::type reportedWaypointPosition;
  Optional<Time>::type reportedWaypointTime;
  Optional<Level>::type reportedWaypointLevel;
  PositionReport() : Sequence(22) {
    Sequence::Inserter it = back_inserter();
    it << positioncurrent << timeatpositioncurrent << level << fixnext << timeetaatfixnext << fixnextplusone << timeetaatdestination << remainingFuel << temperature << winds << turbulence << icing << speed << speedground << verticalChange << trackAngle << heading << distance << humidity << reportedWaypointPosition << reportedWaypointTime << reportedWaypointLevel;
  }
};

class PositionRouteClearanceIndex : public Asn1::Sequence {
public:
  Required<Position>::type position;
  Required<RouteClearanceIndex>::type routeClearanceIndex;
  PositionRouteClearanceIndex() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << position << routeClearanceIndex;
  }
};

class PositionSpeed : public Asn1::Sequence {
public:
  Required<Position>::type position;
  Required<Speed>::type speed;
  PositionSpeed() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << position << speed;
  }
};

class PositionSpeedSpeed : public Asn1::Sequence {
public:
  Required<Position>::type position;
  Required<SpeedSpeed>::type speeds;
  PositionSpeedSpeed() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << position << speeds;
  }
};

class PositionTime : public Asn1::Sequence {
public:
  Required<Position>::type position;
  Required<Time>::type time;
  PositionTime() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << position << time;
  }
};

class PositionTimeLevel : public Asn1::Sequence {
public:
  Required<PositionTime>::type positionTime;
  Required<Level>::type level;
  PositionTimeLevel() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << positionTime << level;
  }
};

class PositionTimeTime : public Asn1::Sequence {
public:
  Required<Position>::type position;
  Required<TimeTime>::type times;
  PositionTimeTime() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << position << times;
  }
};

class PositionUnitNameFrequency : public Asn1::Sequence {
public:
  Required<Position>::type position;
  Required<UnitName>::type unitname;
  Required<Frequency>::type frequency;
  PositionUnitNameFrequency() : Sequence(3) {
    Sequence::Inserter it = back_inserter();
    it << position << unitname << frequency;
  }
};

class ProcedureName : public Asn1::Sequence {
public:
  Required<ProcedureType>::type type;
  Required<Procedure>::type procedure;
  Optional<ProcedureTransition>::type transition;
  ProcedureName() : Sequence(3) {
    Sequence::Inserter it = back_inserter();
    it << type << procedure << transition;
  }
};

class ProcedureType : public Asn1::Enumerated<0,2> {
public:
  virtual bool extensible() const { return false; }
  typedef enum {
    arrival = 0,
    approach = 1,
    departure = 2
  } EnumValues;
  ProcedureType() : Enumerated() { }
  ProcedureType(int value) : Enumerated(value) { }
};

class PublishedIdentifier : public Asn1::Choice {
public:
  typedef PublishedIdentifier PrimitiveType;
  enum {
    fixName_idx = 0,
    navaid_idx = 1
  };
  virtual bool extensible() const { return false; }
  Option<fixName_idx, FixName>::type fixName();
  Option<navaid_idx, Navaid>::type navaid();
  PublishedIdentifier() : Choice() { }
};

class RemainingFuelPersonsOnBoard : public Asn1::Sequence {
public:
  Required<RemainingFuel>::type remainingFuel;
  Required<PersonsOnBoard>::type personsOnBoard;
  RemainingFuelPersonsOnBoard() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << remainingFuel << personsOnBoard;
  }
};

class ReportingPoints : public Asn1::Sequence {
public:
  Required<LatLonReportingPoints>::type latLonReportingPoints;
  Optional<DegreeIncrement>::type degreeIncrement;
  ReportingPoints() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << latLonReportingPoints << degreeIncrement;
  }
};

class RouteAndLevels : public Asn1::Sequence {
public:
  Required<RouteInformation>::type routeOfFlight;
  Required<LevelsOfFlight>::type levelsOfFlight;
  RouteAndLevels() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << routeOfFlight << levelsOfFlight;
  }
};

class RouteClearance : public Asn1::Sequence {
public:
  Optional<Airport>::type airportDeparture;
  Optional<Airport>::type airportDestination;
  Optional<Runway>::type runwayDeparture;
  Optional<ProcedureName>::type procedureDeparture;
  Optional<Runway>::type runwayArrival;
  Optional<ProcedureName>::type procedureApproach;
  Optional<ProcedureName>::type procedureArrival;
  Optional<Asn1::SequenceOf<RouteInformation, 1, 128> >::type routeInformations;
  Optional<RouteInformationAdditional>::type routeInformationAdditional;
  RouteClearance() : Sequence(9) {
    Sequence::Inserter it = back_inserter();
    it << airportDeparture << airportDestination << runwayDeparture << procedureDeparture << runwayArrival << procedureApproach << procedureArrival << routeInformations << routeInformationAdditional;
  }
};

class RouteInformation : public Asn1::Choice {
public:
  typedef RouteInformation PrimitiveType;
  enum {
    publishedIdentifier_idx = 0,
    latitudeLongitude_idx = 1,
    placeBearingPlaceBearing_idx = 2,
    placeBearingDistance_idx = 3,
    aTSRouteDesignator_idx = 4
  };
  virtual bool extensible() const { return false; }
  Option<publishedIdentifier_idx, PublishedIdentifier>::type publishedIdentifier();
  Option<latitudeLongitude_idx, LatitudeLongitude>::type latitudeLongitude();
  Option<placeBearingPlaceBearing_idx, PlaceBearingPlaceBearing>::type placeBearingPlaceBearing();
  Option<placeBearingDistance_idx, PlaceBearingDistance>::type placeBearingDistance();
  Option<aTSRouteDesignator_idx, ATSRouteDesignator>::type aTSRouteDesignator();
  RouteInformation() : Choice() { }
};

class RouteInformationAdditional : public Asn1::Sequence {
public:
  Optional<Asn1::SequenceOf<ATWAlongTrackWaypoint, 1, 8> >::type aTWAlongTrackWaypoints;
  Optional<ReportingPoints>::type reportingpoints;
  Optional<Asn1::SequenceOf<InterceptCourseFrom, 1, 4> >::type interceptCourseFroms;
  Optional<Asn1::SequenceOf<Holdatwaypoint, 1, 8> >::type holdAtWaypoints;
  Optional<Asn1::SequenceOf<WaypointSpeedLevel, 1, 32> >::type waypointSpeedLevels;
  Optional<Asn1::SequenceOf<RTARequiredTimeArrival, 1, 32> >::type rTARequiredTimeArrivals;
  RouteInformationAdditional() : Sequence(6) {
    Sequence::Inserter it = back_inserter();
    it << aTWAlongTrackWaypoints << reportingpoints << interceptCourseFroms << holdAtWaypoints << waypointSpeedLevels << rTARequiredTimeArrivals;
  }
};

class RTARequiredTimeArrival : public Asn1::Sequence {
public:
  Required<Position>::type position;
  Required<RTATime>::type rTATime;
  Optional<RTATolerance>::type rTATolerance;
  RTARequiredTimeArrival() : Sequence(3) {
    Sequence::Inserter it = back_inserter();
    it << position << rTATime << rTATolerance;
  }
};

class RTATime : public Asn1::Sequence {
public:
  Required<Time>::type time;
  Required<TimeTolerance>::type timeTolerance;
  RTATime() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << time << timeTolerance;
  }
};

class Runway : public Asn1::Sequence {
public:
  Required<RunwayDirection>::type direction;
  Required<RunwayConfiguration>::type configuration;
  Runway() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << direction << configuration;
  }
};

class RunwayConfiguration : public Asn1::Enumerated<0,3> {
public:
  virtual bool extensible() const { return false; }
  typedef enum {
    left = 0,
    right = 1,
    center = 2,
    none = 3
  } EnumValues;
  RunwayConfiguration() : Enumerated() { }
  RunwayConfiguration(int value) : Enumerated(value) { }
};

class RunwayRVR : public Asn1::Sequence {
public:
  Required<Runway>::type runway;
  Required<RVR>::type rVR;
  RunwayRVR() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << runway << rVR;
  }
};

class RVR : public Asn1::Choice {
public:
  typedef RVR PrimitiveType;
  enum {
    rVRFeet_idx = 0,
    rVRMeters_idx = 1
  };
  virtual bool extensible() const { return false; }
  Option<rVRFeet_idx, RVRFeet>::type rVRFeet();
  Option<rVRMeters_idx, RVRMeters>::type rVRMeters();
  RVR() : Choice() { }
};

class Speed : public Asn1::Choice {
public:
  typedef Speed PrimitiveType;
  enum {
    speedIndicated_idx = 0,
    speedIndicatedMetric_idx = 1,
    speedTrue_idx = 2,
    speedTrueMetric_idx = 3,
    speedGround_idx = 4,
    speedGroundMetric_idx = 5,
    speedMach_idx = 6
  };
  virtual bool extensible() const { return false; }
  Option<speedIndicated_idx, SpeedIndicated>::type speedIndicated();
  Option<speedIndicatedMetric_idx, SpeedIndicatedMetric>::type speedIndicatedMetric();
  Option<speedTrue_idx, SpeedTrue>::type speedTrue();
  Option<speedTrueMetric_idx, SpeedTrueMetric>::type speedTrueMetric();
  Option<speedGround_idx, SpeedGround>::type speedGround();
  Option<speedGroundMetric_idx, SpeedGroundMetric>::type speedGroundMetric();
  Option<speedMach_idx, SpeedMach>::type speedMach();
  Speed() : Choice() { }
};

class SpeedTime : public Asn1::Sequence {
public:
  Required<Speed>::type speed;
  Required<Time>::type time;
  SpeedTime() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << speed << time;
  }
};

class SpeedType : public Asn1::Enumerated<0,8> {
public:
  virtual bool extensible() const { return true; }
  typedef enum {
    noneSpecified = 0,
    indicated = 1,
    true_ = 2,
    ground = 3,
    mach = 4,
    approach = 5,
    cruise = 6,
    minimum = 7,
    maximum = 8
  } EnumValues;
  SpeedType() : Enumerated() { }
  SpeedType(int value) : Enumerated(value) { }
};

class SpeedTypeSpeedTypeSpeedTypeSpeed : public Asn1::Sequence {
public:
  Required<SpeedTypeSpeedTypeSpeedType>::type speedTypes;
  Required<Speed>::type speed;
  SpeedTypeSpeedTypeSpeedTypeSpeed() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << speedTypes << speed;
  }
};

class Time : public Asn1::Sequence {
public:
  Required<TimeHours>::type hours;
  Required<TimeMinutes>::type minutes;
  Time() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << hours << minutes;
  }
};

class TimeLevel : public Asn1::Sequence {
public:
  Required<Time>::type time;
  Required<Level>::type level;
  TimeLevel() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << time << level;
  }
};

class TimeDeparture : public Asn1::Sequence {
public:
  Optional<Time>::type timeDepartureAllocated;
  Optional<ControlledTime>::type timeDepartureControlled;
  Optional<Time>::type timeDepartureClearanceExpected;
  Optional<DepartureMinimumInterval>::type departureMinimumInterval;
  TimeDeparture() : Sequence(4) {
    Sequence::Inserter it = back_inserter();
    it << timeDepartureAllocated << timeDepartureControlled << timeDepartureClearanceExpected << departureMinimumInterval;
  }
};

class TimeDistanceSpecifiedDirection : public Asn1::Sequence {
public:
  Required<Time>::type time;
  Required<DistanceSpecifiedDirection>::type distanceSpecifiedDirection;
  TimeDistanceSpecifiedDirection() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << time << distanceSpecifiedDirection;
  }
};

class TimeDistanceToFromPosition : public Asn1::Sequence {
public:
  Required<Time>::type time;
  Required<Distance>::type distance;
  Required<ToFrom>::type tofrom;
  Required<Position>::type position;
  TimeDistanceToFromPosition() : Sequence(4) {
    Sequence::Inserter it = back_inserter();
    it << time << distance << tofrom << position;
  }
};

class Timehhmmss : public Asn1::Sequence {
public:
  Required<Time>::type hoursminutes;
  Required<TimeSeconds>::type seconds;
  Timehhmmss() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << hoursminutes << seconds;
  }
};

class TimeUnitNameFrequency : public Asn1::Sequence {
public:
  Required<Time>::type time;
  Required<UnitName>::type unitName;
  Required<Frequency>::type frequency;
  TimeUnitNameFrequency() : Sequence(3) {
    Sequence::Inserter it = back_inserter();
    it << time << unitName << frequency;
  }
};

class TimePosition : public Asn1::Sequence {
public:
  Required<Time>::type time;
  Required<Position>::type position;
  TimePosition() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << time << position;
  }
};

class TimePositionLevel : public Asn1::Sequence {
public:
  Required<TimePosition>::type timeposition;
  Required<Level>::type level;
  TimePositionLevel() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << timeposition << level;
  }
};

class TimePositionLevelSpeed : public Asn1::Sequence {
public:
  Required<TimePosition>::type timeposition;
  Required<LevelSpeed>::type levelspeed;
  TimePositionLevelSpeed() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << timeposition << levelspeed;
  }
};

class TimeSpeed : public Asn1::Sequence {
public:
  Required<Time>::type time;
  Required<Speed>::type speed;
  TimeSpeed() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << time << speed;
  }
};

class TimeSpeedSpeed : public Asn1::Sequence {
public:
  Required<Time>::type time;
  Required<SpeedSpeed>::type speedspeed;
  TimeSpeedSpeed() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << time << speedspeed;
  }
};

class TimeToFromPosition : public Asn1::Sequence {
public:
  Required<Time>::type time;
  Required<ToFrom>::type tofrom;
  Required<Position>::type position;
  TimeToFromPosition() : Sequence(3) {
    Sequence::Inserter it = back_inserter();
    it << time << tofrom << position;
  }
};

class TimeTolerance : public Asn1::Enumerated<0,2> {
public:
  virtual bool extensible() const { return false; }
  typedef enum {
    at = 0,
    atorafter = 1,
    atorbefore = 2
  } EnumValues;
  TimeTolerance() : Enumerated() { }
  TimeTolerance(int value) : Enumerated(value) { }
};

class ToFrom : public Asn1::Enumerated<0,1> {
public:
  virtual bool extensible() const { return false; }
  typedef enum {
    to = 0,
    from = 1
  } EnumValues;
  ToFrom() : Enumerated() { }
  ToFrom(int value) : Enumerated(value) { }
};

class ToFromPosition : public Asn1::Sequence {
public:
  Required<ToFrom>::type toFrom;
  Required<Position>::type position;
  ToFromPosition() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << toFrom << position;
  }
};

class TrafficType : public Asn1::Enumerated<0,5> {
public:
  virtual bool extensible() const { return true; }
  typedef enum {
    noneSpecified = 0,
    oppositeDirection = 1,
    sameDirection = 2,
    converging = 3,
    crossing = 4,
    diverging = 5
  } EnumValues;
  TrafficType() : Enumerated() { }
  TrafficType(int value) : Enumerated(value) { }
};

class Turbulence : public Asn1::Enumerated<0,2> {
public:
  virtual bool extensible() const { return false; }
  typedef enum {
    light = 0,
    moderate = 1,
    severe = 2
  } EnumValues;
  Turbulence() : Enumerated() { }
  Turbulence(int value) : Enumerated(value) { }
};

class UnitName : public Asn1::Sequence {
public:
  Required<FacilityDesignation>::type facilityDesignation;
  Optional<FacilityName>::type facilityName;
  Required<FacilityFunction>::type facilityFunction;
  UnitName() : Sequence(3) {
    Sequence::Inserter it = back_inserter();
    it << facilityDesignation << facilityName << facilityFunction;
  }
};

class UnitNameFrequency : public Asn1::Sequence {
public:
  Required<UnitName>::type unitName;
  Required<Frequency>::type frequency;
  UnitNameFrequency() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << unitName << frequency;
  }
};

class VerticalChange : public Asn1::Sequence {
public:
  Required<VerticalDirection>::type direction;
  Required<VerticalRate>::type rate;
  VerticalChange() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << direction << rate;
  }
};

class VerticalDirection : public Asn1::Enumerated<0,1> {
public:
  virtual bool extensible() const { return false; }
  typedef enum {
    up = 0,
    down = 1
  } EnumValues;
  VerticalDirection() : Enumerated() { }
  VerticalDirection(int value) : Enumerated(value) { }
};

class VerticalRate : public Asn1::Choice {
public:
  typedef VerticalRate PrimitiveType;
  enum {
    verticalRateEnglish_idx = 0,
    verticalRateMetric_idx = 1
  };
  virtual bool extensible() const { return false; }
  Option<verticalRateEnglish_idx, VerticalRateEnglish>::type verticalRateEnglish();
  Option<verticalRateMetric_idx, VerticalRateMetric>::type verticalRateMetric();
  VerticalRate() : Choice() { }
};

class WaypointSpeedLevel : public Asn1::Sequence {
public:
  Required<Position>::type position;
  Optional<Speed>::type speed;
  Optional<ATWLevelSequence>::type aTWLevels;
  WaypointSpeedLevel() : Sequence(3) {
    Sequence::Inserter it = back_inserter();
    it << position << speed << aTWLevels;
  }
};

class Winds : public Asn1::Sequence {
public:
  Required<WindDirection>::type direction;
  Required<WindSpeed>::type speed;
  Winds() : Sequence(2) {
    Sequence::Inserter it = back_inserter();
    it << direction << speed;
  }
};

class WindSpeed : public Asn1::Choice {
public:
  typedef WindSpeed PrimitiveType;
  enum {
    windSpeedEnglish_idx = 0,
    windSpeedMetric_idx = 1
  };
  virtual bool extensible() const { return false; }
  Option<windSpeedEnglish_idx, WindSpeedEnglish>::type windSpeedEnglish();
  Option<windSpeedMetric_idx, WindSpeedMetric>::type windSpeedMetric();
  WindSpeed() : Choice() { }
};

}
