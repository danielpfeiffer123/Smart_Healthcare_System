#include <string>
#include <chrono>
#include <vector>

#ifndef MODEL_H
#define MODEL_H
/**
 * @brief 基本信息 
 * @note 包含年龄、性别、电话号码、地址
 */
struct BaseInfo {
  int age;
  int gender;  // male: 1, female: 0
  std::string id_num;
  std::string phone_number;
  std::string address;
  BaseInfo() : age(0), gender(1), id_num("none"), phone_number("none"), address("none"){};
  BaseInfo(int age, int gender, std::string id_num, std::string phone_number, std::string address) :
    age(age), gender(gender),id_num(id_num), phone_number(phone_number), address(address) {};
};

/**
 * @brief 病例信息 
 * @note 包含患者信息，患者病例，医生姓名，医嘱，处方
 */
struct CaseInfo {
  std::string patient_name;
  std::string patient_case;
  std::string doctor_name;
  std::string medical_advice;
  std::string prescription;
  CaseInfo() : patient_name("none"), patient_case("none"), doctor_name("none"), medical_advice("none"), prescription("none"){};
};

/**
 * @brief 预约信息 
 * @note 包含患者信息，医生信息，预约时间
 */
struct ReservationInfo {
  std::string patient_name;
  std::string doctor_name;
  std::time_t date;
  ReservationInfo() : patient_name("none"), doctor_name("none"), date(-1) {};
};

/**
 * @brief 健康评估 
 * @note 包括身高，体重，心率，血压，肺活量
 */
struct HealthAssessment {
  double height = 0;
  double weight = 0;
  double heart_rate = 0;
  double blood_pressure = 0;
  double lung_capacity = 0;
};

/**
 * @brief 注册信息 
 * @note 包括用户名，密码，身份，基本信息
 */
class RegistrationInfo {
  public:
    RegistrationInfo() {}; // 用于返回不存在用户
    RegistrationInfo(std::string name, std::string pwd, int identify, BaseInfo base_info = BaseInfo()) :
      username(name), pwd(pwd), identify(identify), base_info(base_info) {
        status = true;
        user_count++;
      };
    ~RegistrationInfo() {user_count--;};

    std::string username;
    std::string pwd;
    int identify;         // 0: patient, 1: doctor
    BaseInfo base_info;
    bool status = false;  // 1: exist, 0: not exist
    static int user_count;
};

/**
 * @brief 医生用户 
 * @note 包括预约信息,诊断病例，医生数量
 */
class Doctor : public RegistrationInfo {
  public:
    Doctor() {};
    Doctor(std::string name, std::string pwd, BaseInfo base_info = BaseInfo()) :
      RegistrationInfo(name, pwd, 1, base_info) {doctor_count++;};

    std::string title;
    std::string department;
    int years_of_service;
    std::vector<CaseInfo> dialogs;
    std::vector<ReservationInfo> reservations;
    static int doctor_count;
};

/**
 * @brief 患者用户 
 * @note 包括病例详情，健康评估，预约信息，患者数量
 */
class Patient : public RegistrationInfo {
  public:
    Patient() {};
    Patient(std::string name, std::string pwd, BaseInfo base_info = BaseInfo()) :
      RegistrationInfo(name, pwd, 0, base_info) {patient_count++;};

    std::string birthday;
    std::string allergens;
    HealthAssessment health_assessment;
    std::vector<CaseInfo> case_info;
    ReservationInfo reservation;
    static int patient_count;
};

#endif // MODEL_H