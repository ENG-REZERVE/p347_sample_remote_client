#ifndef _CKDR_H_
#define _CKDR_H_

#include <time.h>

#ifdef __WIN32__
//#include <Windows.h>
#define CKDRAPI  __stdcall
//#define KDRMessage(s) OutputDebugString(s)
#define KDRMessage(s)
#ifdef _WINDLL
#define DllExport __declspec(dllexport)
#else
#define DllExport
#endif
#else
#define DllExport
#define CKDRAPI
#define KDRMessage(s)
#endif

typedef float TFloatKDR;
typedef unsigned int ULongKDR_t;
typedef unsigned short UShortKDR_t;

#define CKDR_STATUS ULongKDR_t
#define KDREXCEPTIONMSGLENGTH 128

#define ckdr_ok                224455200L
#define ckdr_initfault         224455201L
#define ckdr_callbackerror     224455202L
#define ckdr_notparam          224455203L
#define ckdr_bearingnone       224455204L
#define ckdr_bearingerror      224455205L
#define ckdr_bearingpairerror  224455206L
#define ckdr_errortypepoint    224455207L
#define ckdr_bad               224455208L
#define ckdr_errormotorcfg     224455209L
#define ckdr_errorkoeffrotation 224455210L

#define COUNT_DEFNAMEID                       42
#define DEFNAMEID_NONE                        0
#define DEFNAMEID_NID                         1
#define DEFNAMEID_RUNNING_OUT_RING            2
#define DEFNAMEID_NATAG                       3
#define DEFNAMEID_PEREKOS                     4
#define DEFNAMEID_IZNOS_OUT_RING              5
#define DEFNAMEID_RAKOV_OUT_RING              6
#define DEFNAMEID_ISNOS_IN_RING               7
#define DEFNAMEID_RAKOV_IN_RING               8
#define DEFNAMEID_ISNOS_TELKACHENIA_SEPARATOR 9
#define DEFNAMEID_RAKOV_TELKACHENIA_SEPARATOR 10
#define DEFNAMEID_11                          11
#define DEFNAMEID_PROSKALSIVANIE_RING         12
#define DEFNAMEID_SMAZKA                      13
#define DEFNAMEID_BOI_VALA                    14
#define DEFNAMEID_ROTOR                       15
#define DEFNAMEID_USEL_KREPLENIA              16
#define DEFNAMEID_MUFTA                       17
#define DEFNAMEID_ROTOR_MUFTA                 18
#define DEFNAMEID_SMALL_GEAR                  19
#define DEFNAMEID_BIG_GEAR                    20
#define DEFNAMEID_ZACEP_SMALL_GEAR            21
#define DEFNAMEID_ZACEP_BIG_GEAR              22
#define DEFNAMEID_OTHER_AXIS                  23
#define DEFNAMEID_TELKACHENIA_SEPARATOR       24
#define DEFNAMEID_NIDVIB                      25
#define DEFNAMEID_JAKORJA                     26
#define DEFNAMEID_SYS_VIZB                    27
#define DEFNAMEID_COLLECT                     28
#define DEFNAMEID_UPULS                       29
#define DEFNAMEID_OPORA_ROTORA                30
#define DEFNAMEID_STATIC_ECCENTRICITY         31
#define DEFNAMEID_DINAMIC_ECCENTRICITY        32
#define DEFNAMEID_SQUIRREL_CAGE               33
#define DEFNAMEID_STATOR                      34
#define DEFNAMEID_FORMA_U                     35
#define DEFNAMEID_DISSYMETRY_U                36
#define DEFNAMEID_DRIVE_WINDING               37
#define DEFNAMEID_AVTOKOLEBANIA_VALA          38
#define DEFNAMEID_PEREKOS_BEARING             39
#define DEFNAMEID_ISNOS_BEARING               40
#define DEFNAMEID_YDAR_IN_BEARING             41

#define COUNT_RECOMID  31
#define RECOMID_NONE 0
#define RECOMID_UTOCHNIT_DEFECT 1
#define RECOMID_PROVERIT_SMAZKU 2
#define RECOMID_PROVERIT_CENTER_VAL 3
#define RECOMID_USTRANIT_AUTOKOLEB 4
#define RECOMID_PROISVESTI_BALANSIROVKU 5
#define RECOMID_PROISVESTI_REMONT_PODSHIPNIKOVOGO_UZLA 6
#define RECOMID_ZAMENIT_PODSHIPNIK 7
#define RECOMID_USTRANIT_PEREKOS_PODSHIPNIKA 8
#define RECOMID_USTRANIT_NATAG 9
#define RECOMID_PROVERIT_SOSTOANIE_KREPLENIA 10
#define RECOMID_USTRANIT_PRICHINU_UDAROV_V_PODSHIPNIKE 11
#define RECOMID_PROISVESTI_OSMOTR_GEAR 12
#define RECOMID_PROVERIT_GEAR 13
#define RECOMID_UTOCHNIT_DEF_OTHER_AXIS 14
#define RECOMID_USTRANIT_DEF_MUFTA 15
#define RECOMID_PROVERIT_ZAZOR 16
#define RECOMID_17 17
#define RECOMID_18 18
#define RECOMID_19 19
#define RECOMID_20 20
#define RECOMID_21 21
#define RECOMID_22 22
#define RECOMID_23 23
#define RECOMID_PROISVEST_OSMOTR_UZLA 24
#define RECOMID_RECOM_DATA_NEXT_DIAGN 25
#define RECOMID_RESERV1 26
#define RECOMID_RESERV2 27
#define RECOMID_RESERV3 28
#define RECOMID_VIASNIT_PRICHIN_VIBR 29
#define RECOMID_PROISVESTI_POVTOR_MEAS 30

//WARNING_xxx
//������� ����������� ������ ��� ������� ������� ��������.
#define WARNING_ERROR_FROT (1L<<0)

//������� ����������� �������� ������ ��� ��������� � ��� ���������
//������ ��� ��������� �������� �������� ��������� ������.
//������������� �������� ������ � �������� ��������� ���������.
#define WARNING_BAD_OIL (1L<<1)

//����� ����� ��������������� ��� � �������������� �������� ������.
//����������� ��������� ������ �� �������� ���������.
#define WARNING_ONLY_ENV (1L<<2)

//��������!
//������� ��������� ����� ���������������
//��������� �������� ������.
#define WARNING_AUTOMATION (1L<<3)

//������������� ���������� ����������� �������� ���������,
//��� ���������� �������. ���������������� �� ������
//��������������� �������� �� �����������.
#define WARNING_ENV_LITTLE (1L<<4)

//������������� ��� ���������� ������� ����� �������� ���������.
//���������������� �� ������ ��������������� �������� �� �����������.
//������������� ������� ������������ ������ (�������).
#define WARNING_ENV_BAD_HISTORY (1L<<5)

//������������� ���������� ���������� � ������� ���������.
#define WARNING_ENV_AVERAGE (1L<<6)

//��� �������� ��������� ��� ����������������.
#define WARNING_ENV_NONE (1L<<7)

//��� ������ �������� ��� ����������������.
#define WARNING_DIR_NONE (1L<<8)

//������������� ���������� ���������� � ������ �������.
#define WARNING_DIR_AVERAGE (1L<<9)

//��� ���������������� �� ������ �������� ��� ���������� �������
//���� ������������� �� ��� �������.
//�������, ������� �� ����������� ��� ����������
//��������. (������������� �������) :
#define WARNING_DIR_ETALON (1L<<10)

//������������� ���������� ����������� ������ ��������.
//���������������� � ����������� ��������
//�� �����������.
#define WARNING_DIR_LITTLE (1L<<11)

//������������� ��� ���������� ������� ����� ������ ��������.
//���������������� � ����������� ��������  �� �����������.
//������������� ������� ������������ ������ (�������).
#define WARNING_DIR_BAD_HISTORY (1L<<12)

//���������� ������ ��� ��������� ������� ���������.
//(������ ��������� ������� ��� ����������� ���� � ������).
//������������� ��������� ���������.
#define WARNING_ENV_NOISE (1L<<13)

//������� ������� ������������� ������������ � ������� ���������.
//�������� ������ � ����������.
//�������� ������� ������������� ������������
//�������� � ������ ������� ��������� ���������.
#define WARNING_ENV_BIGHARMONICS (1L<<14)

//� ������ ������� ���������� ������� �����, ��������� ������� �
//�������������� ������� �������� �� ����� ���������.
//��� ���������� ������ � �������� ���������� ��������� ���������.
#define WARNING_MsgW06Db (1L<<15)

//--------------------------------------------------------------------------------------------------------------
//--������ ����������� ��� ��������� � ����������� �������������� ��
// ���������� ������� ������� ��� ������� ��������
// � ������� ����� �������� ���� 0-3
// ���� �������� ���� == 0 - ������ �� ����������� (== WARNINGPOROG_NOT_POROGS)
// ����� ���������� ������������� ���� � �����

//�� ����������� ������ � �������� ��������.
//����������� ������� ��������� ���� ��� ������� ����������.
#define WARNINGPOROG_NOT_POROGS (0)

//������ � �������� �������� �� ���������. �������� � �����.
#define WARNINGPOROG_OK (1L<<0)

//�������� ����� "������� �������" � �������� ��������.
#define WARNINGPOROG_D (1L<<1)

//�������� ����� "������� �������" � �������� ��������.
#define WARNINGPOROG_H (1L<<2)

//������ ���� ������ "������ �������" � �������� ��������.
//��������� ������� - ����� �������������� ������.
#define WARNINGPOROG_L (1L<<3)

//��������������� �������
#define IS_WARNINGPOROG_NOT_POROGS(fb)  ((((fb >> 16) & 0x0F) | WARNINGPOROG_NOT_POROGS) == 0)
#define IS_WARNINGPOROG_OK(fb)          ((((fb >> 16) & 0x0F) & WARNINGPOROG_OK) != 0)
#define IS_WARNINGPOROG_D(fb)           ((((fb >> 16) & 0x0F) & WARNINGPOROG_D) != 0)
#define IS_WARNINGPOROG_H(fb)           ((((fb >> 16) & 0x0F) & WARNINGPOROG_H) != 0)
#define IS_WARNINGPOROG_L(fb)           ((((fb >> 16) & 0x0F) & WARNINGPOROG_L) != 0)

#define SET_WARNINGPOROG_NOT_POROGS(fb) (fb |= (WARNINGPOROG_NOT_POROGS << 16))
#define SET_WARNINGPOROG_OK(fb)         (fb |= (WARNINGPOROG_OK << 16))
#define SET_WARNINGPOROG_D(fb)          (fb |= (WARNINGPOROG_D << 16))
#define SET_WARNINGPOROG_H(fb)          (fb |= (WARNINGPOROG_H << 16))
#define SET_WARNINGPOROG_L(fb)          (fb |= (WARNINGPOROG_L << 16))

//----------------------------------------------------------------------------------------

typedef struct {
  unsigned int id;
  char verstr[256];
}kdrVersionLib_t;

/**��� ����� ���������
  tpBEARING         - ��������� ������� ����
  tpBEARING_REDUCER - ��������� ������� ���������
  tpMOTOR_2_0       - ����������� ���������
  tpMOTOR_2_1       - ���������� ������
  tpMOTOR_2_2       - ������ ����������� ����
  tpBearingSleeve   - ��������� ����������
*/
//typedef enum {kdrtpBEARING = 0, kdrtpBEARING_REDUCER} typepoint_t;
#define kdrtpUnknown 0
#define kdrtpBEARING 1
#define kdrtpBEARING_REDUCER 2
#define kdrtpMOTOR_2_0 3
#define kdrtpMOTOR_2_1 4
#define kdrtpMOTOR_2_2 5
#define kdrtpBearingSleeve 6
typedef UShortKDR_t typepoint_t;

/**��� ����������
*/
typedef struct {
  TFloatKDR din;                    //������� ����������� ������ [��]
  TFloatKDR dout;                   //������� ��������� ������ [��]
  TFloatKDR angl;                   //���� �������� [��]
  TFloatKDR drol;                   //������� ��� ������� [��]
  UShortKDR_t nrol;             //���-�� ��� �������
  UShortKDR_t dnrol;
}kdrBearing_t;

//����� ��� ������� ������� ������ ������
/**��� ���������
*/
typedef struct {
  UShortKDR_t axis;             //����� ��� ����������
  UShortKDR_t z12;
  UShortKDR_t z21;
  UShortKDR_t z23;
  UShortKDR_t z32;
  UShortKDR_t z34;
  UShortKDR_t z43;
  UShortKDR_t z45;
  UShortKDR_t z54;
}kdrReducer_t;

typedef struct {
  UShortKDR_t L;
  UShortKDR_t R;
}kdrMNTRFrq_t;

typedef kdrMNTRFrq_t kdrMNTRFrqTab_t[101];
typedef enum {kdrLtD = 0, kdrLtH, kdrLtL, kdrLtGD, kdrLtGH, kdrLtGL} kdrLevelType_t;
typedef TFloatKDR kdrSpLevel_t[101];

typedef struct {
  UShortKDR_t nCh;                                //���-�� �����
  kdrMNTRFrqTab_t FrqTab;                         //������� ������
  kdrSpLevel_t Level[3];                          //������ (array[kdrLtD..kdrLtL])
}kdrMNTRLevels_t;

typedef struct {
  UShortKDR_t mpgroup;                     //����� ������
  UShortKDR_t dirNline;                    //���-�� ������� � �������
  TFloatKDR dirFs;                         //���
  //bool IsTangensMp;                      //C ������������� ������������?
  kdrMNTRLevels_t MNTRLevels;              //������
}kdrPorogDirData_t;

#define KDR_MAXLINEINSPECTR 1600
typedef TFloatKDR kdrSpectLine_t;
typedef kdrSpectLine_t kdrSpectLineA_t[KDR_MAXLINEINSPECTR+1];

/**��� �������� �������*/
typedef struct {
  time_t date;                        //���� �������
  TFloatKDR frot;                     //������� �������
  TFloatKDR fb;                       //??
  TFloatKDR fs;                       //��� �������
  UShortKDR_t fmtype;                 //��� ���������� �������
  TFloatKDR fm;                       //����������� ������� ���������� �������
  UShortKDR_t nline;                  //���-�� ����� (�������) �������
  UShortKDR_t averenge;               //���-�� ���������� � ������ �������
  kdrSpectLine_t *spectr;             //��������� �� ������ �������� ������������ �����
}kdrSpectr_t;

/**��� �������� ������������ ��������*/
typedef struct {
  kdrSpectr_t env;                    //������ ���������
  kdrSpectr_t *envZoom;               //�������������� ������ ���������
  kdrSpectr_t dir;                    //������ ������
  kdrSpectr_t *dirZoom;               //�������������� ������ ������
  kdrSpectr_t *wideband;              //�������������� ������
  UShortKDR_t countEnvHistory;        //���������� �������� � ������� ���������
  kdrSpectr_t *envHistory;            //������� ���������
  UShortKDR_t countDirHistory;        //���������� �������� � ������� �������
  kdrSpectr_t *dirHistory;            //������� �������
}kdrSpectrPack_t;

/**��� �������� ����������*/
typedef struct {
  TFloatKDR max;                      //������������, �/���2
  TFloatKDR min;                      //�����������, �/���2
  TFloatKDR std;                      //������������������, �/���2
  TFloatKDR kurtosis;                 //����.���������
  TFloatKDR peakFactor;               //���-������
}kdrStatis_t;

/**��������� �������*/
typedef struct
{
  size_t SIZE;                        //������ ��������� kdrEtalonData_t
  int version;                        //������ ��������� kdrEtalonData_t
  UShortKDR_t mpType;                 //��� ����� ���������
  UShortKDR_t mpSubType;              //������ ����� ���������
  struct{
    UShortKDR_t nMidlValue;           //���-�� �������� � �������
    UShortKDR_t nBeginMidleSp;        //������ � ����� ������� �������� �� �������
    UShortKDR_t CountMidleSp;
    TFloatKDR level[500];             //������ ������� [��]
    ULongKDR_t lelevOpt[500];         //����� �������������� �������
  }dirEtalon;
  struct{
    TFloatKDR FirstLineMO;            //��������� �������� ������ ������ ��������
  }envEtalon;
}kdrEtalonData_t;

/**��������� ������� ��� ��������*/
typedef struct {
 UShortKDR_t dirEtalonCalcLevel;      //��� ������ �������� ������� �� ������� �������
 UShortKDR_t idResStr[500];           //id ��������� �������� (������������ �������� ������� ��������� ���������)
 struct{
   int count;
   time_t *date;
 }dirNotCompatibleList;
 kdrEtalonData_t etalon;
}kdrExportEtalonData_t;

typedef struct {
  time_t date;                        //���� ��������� ������� 
  TFloatKDR frot;                     //������� �������
  kdrSpectLine_t *spectr;             //��������� �� ������ �������� ������������ �����
}kdrSpectrData_t;

/**��������� �������� �������� ������� ���������*/
typedef struct {
  time_t date;                        //���� ���������� ���������
  kdrStatis_t statis;                 //���������� �� ������������
  kdrSpectrData_t *wideband;          //�������� ��������������� �������
  kdrSpectrData_t *dir;               //�������� ������� �������
  kdrSpectrData_t *env;               //�������� ������� ���������
  kdrSpectrData_t *dirZoom;           //�������� ��������������� ������� �������
  kdrSpectrData_t *envZoom;           //�������� ��������������� ������� ���������
}kdrItemHistory_t;

/**��������� ������� ���������*/
typedef struct {
  //������������ ���� ����� ��������� � �� ���������
  typepoint_t typePoint;              //��� �����
  kdrBearing_t bearing;               //�������� ���������
  kdrBearing_t bearingPair;           //������ ���������
  kdrReducer_t reducer;               //��������

  //����� ��������� ��� ������� ���� �������
  //TFloatKDR fs;                       //��� �������
  //UShortKDR_t fmtype;                 //��� ���������� �������
  //TFloatKDR fm;                       //����������� ������� ���������� �������
  //UShortKDR_t nline;                  //���-�� ����� (�������) �������
  //UShortKDR_t averenge;               //���-�� ���������� � ������ �������

  //�������
  int Count;                          //���������� ��������� � �������
  kdrItemHistory_t *itemHistory;      //������ ��������� ������� ���������
}kdrHistory_t;

/**��������� ������� ���������� ��� ����������� ����������� �������*/
typedef struct {
  typepoint_t typePoint;              //��� �����
  //��������� ��� ����������� ���������
  UShortKDR_t p;                      //����� ��� �������
  UShortKDR_t z;                      //����� ������ �����
  UShortKDR_t c;                      //����� ������� ����������

  //��������� ��� ����������� ����������� � �������� �������
  UShortKDR_t resursWorkBearing;      //������ ����������
  kdrBearing_t bearing;               //�������� ���������
  kdrBearing_t bearingPair;           //������ ���������
  kdrReducer_t reducer;               //��������
  UShortKDR_t envPorog[21];           //������ �������� �� ��������� array[1..20]
  UShortKDR_t dirPorog[21];           //������ �������� �� ������� array[1..20]
  kdrPorogDirData_t *porogDirData;    //������ ������� ������� �������
  TFloatKDR kurtosisPorog;            //����� �� ������� ���������
  TFloatKDR ErrorFrot;                //������ ��� ������� ��������
  TFloatKDR MaxErrorFrot;             //������������ ������ ��� ������� ��������
  TFloatKDR koeffRotation;            //�����. ��������� ������� �������� (�� ���� ����������� ��������)
  kdrStatis_t *statis;                //���������� ������������
  kdrEtalonData_t *etalon;            //������
  kdrSpectrPack_t specrPack;          //�������
}kdrINParams_t;

typedef struct                        //��������� �������� ������������� ������������ ����� ������� �������
{                                    
  TFloatKDR freq;                     //������� ���������, � ��
  TFloatKDR amp;                      //��������� ������������� ���� (�� ������ ����), � dB
  TFloatKDR ampDb;                    //���������� ��������� ����, � dB
  TFloatKDR modul;                    //% ������� ���������
  char *comment;                      //���������� (�������������� �����)
  ULongKDR_t optDef;                  //����� �������, ����������� �������������� ��������� ������� �� �������
}kdrSpLine_t;

typedef struct                         //��������� ���������� ����������������
{
  ULongKDR_t recommendations;          //������� ��� ������������ �� ��������
  ULongKDR_t optRecommendations[33];   //��� ������������ ��� ������� �������
  UShortKDR_t defects_names[33];       //���������� ��� �������� �������
  TFloatKDR level[33];                 //������� �������. ����������� � bool dB[33] (���� true �� � dB ����� � % mfi)
  char sm[33];                         //������� ������� ("I" - ������, "M" - �������, "S" - �������, "N" - �� ����������������, " " - ��� �������)
  UShortKDR_t ver[33];                 //����������� (�� 20% �� 90%)
  bool dB[33];                         //��������� �� ������� ��������� ������ �������
  ULongKDR_t envOptDef[33];            //������ ������� ������� - ���������� ������������� ������� ��� �������� ��������� (������ � optDef kdrSpLine_t)
  ULongKDR_t dirOptDef[33];            //������ ������� ������� - ���������� ������������� ������� ��� ������ �������� (������ � optDef kdrSpLine_t)
  TFloatKDR selectRotate;              //��������� ��� ����������� ������� ��������
  time_t timeDiagn;                    //����/����� ���������� ����������� �����������
  time_t periodNextMeasure;            //������������� ������ ���������� ���������� ���������/����������� [�������]
  bool isDiagnFull;                    //���� = true - "������� �� ������ (��. ��������������)."
  ULongKDR_t warnings;                 //������� ����� �������������� (��. ��������� WARNING_xxx & WARNINGPOROGS_xxx)
  struct{
    bool isFailure;                    //���� = true - ���� ��������� ��� ������ ���������� �� ���������� ������� ���������
    TFloatKDR value;                   //!= 0 - �������� ������� ���������
  }kurtosis_t;
}kdrMainWindow_t;

typedef struct                        //�������� 
{
  UShortKDR_t nlines;                 //���������� ������������ �����
  kdrSpLine_t *SpLines;               //������ ������������ �����
  TFloatKDR level[33];
  char sm[33];
  UShortKDR_t ver[33];
  bool dB[33];
}kdrSinglWindow_t;

/**��������� �������� ����������*/
typedef struct
{
  kdrMainWindow_t PlotMainWindow;     //��������� ���������(�����������)
                                      //������������, ������ ������������ ��������, ��������������
  kdrSinglWindow_t *lpPlotEnvWindow;  //��������� ������� ��������� �������� � ��������
  kdrSinglWindow_t *lpPlotDirWindow;  //��������� ������� �������� �������� � ��������
}kdrOutParams_t;

#ifdef __WIN32__
typedef CKDR_STATUS (__stdcall *LPFNDLLINIT)(kdrVersionLib_t&);
typedef CKDR_STATUS (__stdcall *LPFNDLLEXIT)(void);
#else
typedef CKDR_STATUS (*LPFNDLLINIT)(kdrVersionLib_t&);
typedef CKDR_STATUS (*LPFNDLLEXIT)(void);
#endif

/**��� callback ������� ���������
  �������� ��� ���������:
  1. isException - ����:
       true - �� ����� ��������� ��������� ����������
  1. isComplete - ����:
       true - ��������� �� �������� ���� ��������� ������ (����� �������� �������� ������)
  2. percent - ������� ����������� ������
  3. data - �������� ��������� ����������� (��������� (������ ��� ���) ������������� ������?)
  4. etalonEx - ������ �������� �������
  5. userdata - ���������������� ������ (�������� ���������� ��� ���������� ������ �� �����������)
  6. msgException - ��������� �������������� �������� ��� ���������� �����������

  ��������� ������ �������� �������:
    �� ��������� ���������� ������� ���������� ������������������ � ������� callback �������
    ��� ���� ������������ ��������� kdrProgress_t ��������
    isComplete = true, ���� etalonEx ��������� �� ��������� �������.
*/
typedef struct
{
  bool isException;
  bool isComplete;
  TFloatKDR percent;
  kdrOutParams_t* data;
  kdrExportEtalonData_t *etalonEx;
  ULongKDR_t userdata;
  char msgException[KDREXCEPTIONMSGLENGTH];
}kdrProgress_t;

#ifdef __WIN32__
typedef void (__stdcall *LPCALLBACKPROGRESS)(kdrProgress_t&);
#else
typedef void (*LPCALLBACKPROGRESS)(kdrProgress_t&);
#endif

/**�������� ������ �� ����������� ��� ����������� �������
 INParams - ��������� �� ������� ��������� �����������
 userdata - ���������������� ������;
 callbackprogress - ������ calback �������
 IsFullDiagn - ���� ������� ����������� ���������� ���������(�����������)
             = true - � ��������� ���������� ������ ������������ �������� � ��������� � ��������
             = false - � ��������� �� ����������� ������ ������������ ��������
*/
typedef struct
{
  kdrINParams_t *lpINParams;
  ULongKDR_t userdata;
  LPCALLBACKPROGRESS callbackprogress;
  bool IsFullDiagn;
}kdrTaskD_t;

#ifdef __WIN32__
typedef CKDR_STATUS (CKDRAPI *LPFNDLLADDDIAG)(kdrTaskD_t&);
typedef CKDR_STATUS (CKDRAPI *LPFNMAKECOMMENTDIGN)(char *, kdrOutParams_t*, time_t &aDateDiagn);
typedef CKDR_STATUS (CKDRAPI *LPFNMAKEXML)(char *, int, kdrOutParams_t&, time_t &aDateDiagn);
#else
typedef CKDR_STATUS (*LPFNDLLADDDIAG)(kdrTaskD_t&);
typedef CKDR_STATUS (*LPFNMAKECOMMENTDIGN)(char *, kdrOutParams_t*);
typedef CKDR_STATUS (*LPFNMAKEXML)(char *, int, kdrOutParams_t&);
#endif

//#ifdef __WIN32__
//#ifdef _WINDLL
#ifdef __cplusplus
extern "C" {
#endif
DllExport CKDR_STATUS CKDRAPI ckdrInit(kdrVersionLib_t&);
DllExport CKDR_STATUS CKDRAPI ckdrAddD(kdrTaskD_t&);
DllExport CKDR_STATUS CKDRAPI ckdrExit(void);
DllExport CKDR_STATUS CKDRAPI ckdrMakeCommentDiagn(char *, kdrOutParams_t*, time_t &aDateDiagn);
DllExport CKDR_STATUS CKDRAPI ckdrMakeCommentDiagn2(char *, kdrOutParams_t*, time_t &aDateDiagn);
DllExport CKDR_STATUS CKDRAPI ckdrMakeXML(char *, int, kdrOutParams_t&, time_t &aDateDiagn);
#ifdef __cplusplus
}
#endif
//#endif
//#endif

#endif
