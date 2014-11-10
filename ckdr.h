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
//Высокая вероятность ошибки при задании частоты вращения.
#define WARNING_ERROR_FROT (1L<<0)

//Высокая вероятность старения смазки или попадания в нее инородных
//частиц при отсутсвии развитых дефектов наружного кольца.
//Рекомендуется заменить смазку и провести повторное измерение.
#define WARNING_BAD_OIL (1L<<1)

//Малый вклад диагностируемой оси в низкочастотную вибрацию машины.
//Диагностика проведена только по спектрам огибающей.
#define WARNING_ONLY_ENV (1L<<2)

//ВНИМАНИЕ!
//Диагноз поставлен после автоматического
//уточнения исходных данных.
#define WARNING_AUTOMATION (1L<<3)

//Недостаточное количество совместимых спектров огибающей,
//для построения эталона. Диагностирование по уровню
//высокочастотной вибрации НЕ ПРОВОДИЛОСЬ.
#define WARNING_ENV_LITTLE (1L<<4)

//Несовместимый для построения эталона набор спектров огибающей.
//Диагностирование по уровню высокочастотной вибрации НЕ ПРОВОДИЛОСЬ.
//Рекомендуется удалить сомнительный спектр (спектры).
#define WARNING_ENV_BAD_HISTORY (1L<<5)

//Недостаточное количество усреднений в спектре огибающей.
#define WARNING_ENV_AVERAGE (1L<<6)

//Нет спектров огибающей для диагностирования.
#define WARNING_ENV_NONE (1L<<7)

//Нет прямых спектров для диагностирования.
#define WARNING_DIR_NONE (1L<<8)

//Недостаточное количество усреднений в прямом спектре.
#define WARNING_DIR_AVERAGE (1L<<9)

//При диагностировании по прямым спектрам для построения эталона
//были использованны не все спектры.
//Спектры, которые не учитывались при вычислении
//эталонов. (Несовместимые спектры) :
#define WARNING_DIR_ETALON (1L<<10)

//Недостаточное количество совместимых прямых спектров.
//Диагностирование с построением эталонов
//НЕ ПРОВОДИЛОСЬ.
#define WARNING_DIR_LITTLE (1L<<11)

//Несовместимый для построения эталона набор прямых спектров.
//Диагностирование с построением эталонов  НЕ ПРОВОДИЛОСЬ.
//Рекомендуется удалить сомнительный спектр (спектры).
#define WARNING_DIR_BAD_HISTORY (1L<<12)

//Обнаружены помехи при измерении спектра огибающей.
//(Плохое крепление датчика или посторонние шумы в машине).
//Рекомендуется повторить измерение.
#define WARNING_ENV_NOISE (1L<<13)

//Высокий уровень гармонических составляющих в спектре огибающей.
//Возможны ошибки в измерениях.
//Возможны сильные гармонические составляющие
//вибрации в полосе фильтра детектора огибающей.
#define WARNING_ENV_BIGHARMONICS (1L<<14)

//В прямом спектре обнаружены широкие линий, возможная причина –
//нестабильность частоты вращения во время измерения.
//Для исключения ошибок в диагнозе необходимо повторное измерение.
#define WARNING_MsgW06Db (1L<<15)

//--------------------------------------------------------------------------------------------------------------
//--Группа определений для выделения и определений предупреждений по
// превышению порогов уровней для спектра вибрации
// в битовой маске занимают биты 0-3
// если значение поля == 0 - пороги не установлены (== WARNINGPOROG_NOT_POROGS)
// иначе необходимо анализировать биты в маске

//Не установлены пороги в спектрах вибрации.
//Достоверный прогноз состояния узла без порогов НЕВОЗМОЖЕН.
#define WARNINGPOROG_NOT_POROGS (0)

//Пороги в спектрах вибрации не превышены. Вибрация в норме.
#define WARNINGPOROG_OK (1L<<0)

//Превышен порог "Опасный уровень" в спектрах вибрации.
#define WARNINGPOROG_D (1L<<1)

//Превышен порог "Сильный уровень" в спектрах вибрации.
#define WARNINGPOROG_H (1L<<2)

//Сигнал ниже порога "Низкий уровень" в спектрах вибрации.
//Возможная причина - отказ измерительного тракта.
#define WARNINGPOROG_L (1L<<3)

//вспомогательные макросы
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

/**тип точки измерения
  tpBEARING         - подшипник качения вала
  tpBEARING_REDUCER - подшипник качения редуктора
  tpMOTOR_2_0       - ассихронный двигатель
  tpMOTOR_2_1       - синхронная машина
  tpMOTOR_2_2       - машина постоянного тока
  tpBearingSleeve   - подшипник скольжения
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

/**тип подшипника
*/
typedef struct {
  TFloatKDR din;                    //диаметр внутреннего кольца [мм]
  TFloatKDR dout;                   //диаметр наружного кольца [мм]
  TFloatKDR angl;                   //угол контакта [мм]
  TFloatKDR drol;                   //диаметр тел качения [мм]
  UShortKDR_t nrol;             //кол-во тел качения
  UShortKDR_t dnrol;
}kdrBearing_t;

//номер оси датчика оборота ВСЕГДА первый
/**тип редуктора
*/
typedef struct {
  UShortKDR_t axis;             //номер оси подшипника
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
  UShortKDR_t nCh;                                //кол-во полос
  kdrMNTRFrqTab_t FrqTab;                         //Таблица частот
  kdrSpLevel_t Level[3];                          //Уровни (array[kdrLtD..kdrLtL])
}kdrMNTRLevels_t;

typedef struct {
  UShortKDR_t mpgroup;                     //Номер группы
  UShortKDR_t dirNline;                    //кол-во каналов в спектре
  TFloatKDR dirFs;                         //ФНЧ
  //bool IsTangensMp;                      //C тангенсальным направлением?
  kdrMNTRLevels_t MNTRLevels;              //Уровни
}kdrPorogDirData_t;

#define KDR_MAXLINEINSPECTR 1600
typedef TFloatKDR kdrSpectLine_t;
typedef kdrSpectLine_t kdrSpectLineA_t[KDR_MAXLINEINSPECTR+1];

/**тип описание спектра*/
typedef struct {
  time_t date;                        //дата спектра
  TFloatKDR frot;                     //обороты спектра
  TFloatKDR fb;                       //??
  TFloatKDR fs;                       //ФНЧ спектра
  UShortKDR_t fmtype;                 //тип полосового фильтра
  TFloatKDR fm;                       //центральная частота полосового фильтра
  UShortKDR_t nline;                  //Кол-во линий (каналов) спектра
  UShortKDR_t averenge;               //кол-во усреднений в снятом спектре
  kdrSpectLine_t *spectr;             //указатель на массив значений спектральных линий
}kdrSpectr_t;

/**тип описания совокупности спектров*/
typedef struct {
  kdrSpectr_t env;                    //спектр огибающей
  kdrSpectr_t *envZoom;               //дополнительный спектр огибающей
  kdrSpectr_t dir;                    //прямой спектр
  kdrSpectr_t *dirZoom;               //дополнительный прямой спектр
  kdrSpectr_t *wideband;              //широкополосный спектр
  UShortKDR_t countEnvHistory;        //количество спектров в истории огибающей
  kdrSpectr_t *envHistory;            //история огибающей
  UShortKDR_t countDirHistory;        //количество спектров в истории прямого
  kdrSpectr_t *dirHistory;            //история прямого
}kdrSpectrPack_t;

/**тип описания статистики*/
typedef struct {
  TFloatKDR max;                      //Максимальное, м/сек2
  TFloatKDR min;                      //Минимальное, м/сек2
  TFloatKDR std;                      //Среднеквадратичное, м/сек2
  TFloatKDR kurtosis;                 //Коеф.Куртозиса
  TFloatKDR peakFactor;               //Пик-фактор
}kdrStatis_t;

/**Структура эталона*/
typedef struct
{
  size_t SIZE;                        //размер структуры kdrEtalonData_t
  int version;                        //версия структуры kdrEtalonData_t
  UShortKDR_t mpType;                 //тип точки измерения
  UShortKDR_t mpSubType;              //подтип точки измерения
  struct{
    UShortKDR_t nMidlValue;           //кол-во значений в эталоне
    UShortKDR_t nBeginMidleSp;        //начало и длина средних значений по спектру
    UShortKDR_t CountMidleSp;
    TFloatKDR level[500];             //уровни эталона [дБ]
    ULongKDR_t lelevOpt[500];         //флаги принадлежности уровней
  }dirEtalon;
  struct{
    TFloatKDR FirstLineMO;            //эталонное значение Общего уровня вибрации
  }envEtalon;
}kdrEtalonData_t;

/**Структура эталона для экспорта*/
typedef struct {
 UShortKDR_t dirEtalonCalcLevel;      //код ошибки создания эталона по прямому спектру
 UShortKDR_t idResStr[500];           //id строковых ресурсов (соответствие значения эталона строковой константе)
 struct{
   int count;
   time_t *date;
 }dirNotCompatibleList;
 kdrEtalonData_t etalon;
}kdrExportEtalonData_t;

typedef struct {
  time_t date;                        //дата измерения спектра 
  TFloatKDR frot;                     //обороты спектра
  kdrSpectLine_t *spectr;             //указатель на массив значений спектральных линий
}kdrSpectrData_t;

/**Структура описания элемента истории измерения*/
typedef struct {
  time_t date;                        //дата проведения измерения
  kdrStatis_t statis;                 //статистика по вибросигналу
  kdrSpectrData_t *wideband;          //значения широкополосного спектра
  kdrSpectrData_t *dir;               //значения прямого спектра
  kdrSpectrData_t *env;               //значения спектра огибающей
  kdrSpectrData_t *dirZoom;           //значения дополнительного прямого спектра
  kdrSpectrData_t *envZoom;           //значения дополнительного спектра огибающей
}kdrItemHistory_t;

/**Структура истории измерений*/
typedef struct {
  //конфигурация типа точки измерения и ее геометрии
  typepoint_t typePoint;              //тип точки
  kdrBearing_t bearing;               //основной подшипник
  kdrBearing_t bearingPair;           //парный подшипник
  kdrReducer_t reducer;               //редуктор

  //общие параметры для каждого типа спектра
  //TFloatKDR fs;                       //ФНЧ спектра
  //UShortKDR_t fmtype;                 //тип полосового фильтра
  //TFloatKDR fm;                       //центральная частота полосового фильтра
  //UShortKDR_t nline;                  //Кол-во линий (каналов) спектра
  //UShortKDR_t averenge;               //кол-во усреднений в снятом спектре

  //история
  int Count;                          //количество элементов в истории
  kdrItemHistory_t *itemHistory;      //массив элементов истории измерений
}kdrHistory_t;

/**Структура входных параметров для диагностики подшипников качения*/
typedef struct {
  typepoint_t typePoint;              //тип точки
  //параметры для диагностики двигателя
  UShortKDR_t p;                      //Число пар полюсов
  UShortKDR_t z;                      //Число зубцов якоря
  UShortKDR_t c;                      //Число пластин коллектора

  //параметры для диагностики подшипников и зубчатых передач
  UShortKDR_t resursWorkBearing;      //ресурс подшипника
  kdrBearing_t bearing;               //основной подшипник
  kdrBearing_t bearingPair;           //парный подшипник
  kdrReducer_t reducer;               //редуктор
  UShortKDR_t envPorog[21];           //пороги дефектов по огибающей array[1..20]
  UShortKDR_t dirPorog[21];           //пороги дефектов по прямому array[1..20]
  kdrPorogDirData_t *porogDirData;    //пороги уровней прямого спектра
  TFloatKDR kurtosisPorog;            //порог по фактору Куртозиса
  TFloatKDR ErrorFrot;                //ошибка при задании оборотов
  TFloatKDR MaxErrorFrot;             //максимальная ошибка при задании оборотов
  TFloatKDR koeffRotation;            //коэфф. пересчета частоты вращения (на него УМНОЖАЕСТСЯ оборотка)
  kdrStatis_t *statis;                //статистика вибросигнала
  kdrEtalonData_t *etalon;            //Эталон
  kdrSpectrPack_t specrPack;          //спектры
}kdrINParams_t;

typedef struct                        //структура описания гармонической составляющей после разбора спектра
{                                    
  TFloatKDR freq;                     //частота гармоники, в Гц
  TFloatKDR amp;                      //амплитуда обнаруженного пика (от уровня фона), в dB
  TFloatKDR ampDb;                    //абсолютная амплитуда пика, в dB
  TFloatKDR modul;                    //% глубины модуляции
  char *comment;                      //коментарий (принадлежность рядам)
  ULongKDR_t optDef;                  //маска битовая, указывающая принадлежность гармоники дефекту по таблице
}kdrSpLine_t;

typedef struct                         //структура результата диагностирования
{
  ULongKDR_t recommendations;          //битовый код рекомендации по таблицам
  ULongKDR_t optRecommendations[33];   //код рекомендации для каждого дефекта
  UShortKDR_t defects_names[33];       //уникальный код названия дефекта
  TFloatKDR level[33];                 //уровень дефекта. размерность в bool dB[33] (если true то в dB иначе в % mfi)
  char sm[33];                         //степень дефекта ("I" - слабый, "M" - средний, "S" - сильный, "N" - не диагностировался, " " - нет дефекта)
  UShortKDR_t ver[33];                 //вероятность (от 20% до 90%)
  bool dB[33];                         //указатель на единицы измерения уровня дефекта
  ULongKDR_t envOptDef[33];            //каждый элемент массива - уникальный идентификатор дефекта для спектров огибающей (связка с optDef kdrSpLine_t)
  ULongKDR_t dirOptDef[33];            //каждый элемент массива - уникальный идентификатор дефекта для прямых спектров (связка с optDef kdrSpLine_t)
  TFloatKDR selectRotate;              //выбранная для диагностики частота вращения
  time_t timeDiagn;                    //дата/время заполнения результатов диагностики
  time_t periodNextMeasure;            //рекомендуемый период проведения следующего измерения/диагностики [секунды]
  bool isDiagnFull;                    //флаг = true - "Диагноз не полный (См. ПРЕДУПРЕЖДЕНИЯ)."
  ULongKDR_t warnings;                 //битовая маска предупреждений (см. константы WARNING_xxx & WARNINGPOROGS_xxx)
  struct{
    bool isFailure;                    //флаг = true - есть основание для замены подшипника по превышению Фактора Куртозиса
    TFloatKDR value;                   //!= 0 - значение Фактора Куртозиса
  }kurtosis_t;
}kdrMainWindow_t;

typedef struct                        //описание 
{
  UShortKDR_t nlines;                 //количество спектральных линий
  kdrSpLine_t *SpLines;               //массив спектральных линий
  TFloatKDR level[33];
  char sm[33];
  UShortKDR_t ver[33];
  bool dB[33];
}kdrSinglWindow_t;

/**Структура выходных параметров*/
typedef struct
{
  kdrMainWindow_t PlotMainWindow;     //результат обработки(диагностики)
                                      //рекомендации, список обнаруженных дефектов, предупреждения
  kdrSinglWindow_t *lpPlotEnvWindow;  //гармоники спектра огибающей привязка к дефектам
  kdrSinglWindow_t *lpPlotDirWindow;  //гармоники спектра вибрации привязка к дефектам
}kdrOutParams_t;

#ifdef __WIN32__
typedef CKDR_STATUS (__stdcall *LPFNDLLINIT)(kdrVersionLib_t&);
typedef CKDR_STATUS (__stdcall *LPFNDLLEXIT)(void);
#else
typedef CKDR_STATUS (*LPFNDLLINIT)(kdrVersionLib_t&);
typedef CKDR_STATUS (*LPFNDLLEXIT)(void);
#endif

/**тип callback функции прогресса
  передает три параметра:
  1. isException - флаг:
       true - во время обработки произошло исключение
  1. isComplete - флаг:
       true - указывает на конечную фазу обработки данных (можно забирать выходные данные)
  2. percent - процент проделанной работы
  3. data - выходные параметры диагностики (структуру (память под нее) предоставляет клиент?)
  4. etalonEx - данные экспорта эталона
  5. userdata - пользовательские данные (значение получаемое при постановки задачи на диагностику)
  6. msgException - сообщение исключительной ситуации при проведении диагностики

  обработка данных экспорта эталона:
    по окончании вычисления эталона вызывается зарегестрированная в системе callback функция
    все поля возвращаемой структуры kdrProgress_t сброшены
    isComplete = true, поле etalonEx указывает на структуру эталона.
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

/**описание задачи на диагностику для подшипников качения
 INParams - указатель на входные параметры диагностики
 userdata - пользовательские данные;
 callbackprogress - адресс calback функции
 IsFullDiagn - флаг степени детализации результата обработки(диагностики)
             = true - в результат заполяются списки обнаруженных гармоник с привязкой к дефектам
             = false - в результат НЕ ЗАПОЛНЯЮТСЯ списки обнаруженных гармоник
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
