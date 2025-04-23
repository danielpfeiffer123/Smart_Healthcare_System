#include <iostream>
#include <thread>
#include <sqlite3.h>
#include <chrono>

#include "database.h"
#include "model.h" 

int main() {
  DataBase db;
  Doctor doc;

  // 医生添加数据测试
  BaseInfo base_info;
  base_info.address = "北京市海淀区";
  base_info.age = 30;
  base_info.gender = 1;
  base_info.id_num = "114514";
  base_info.phone_number = "12345678901";
  Doctor doctor("张三", "pwd", base_info);
  doctor.title = "主任";
  doctor.department = "外科";
  doctor.years_of_service = 11;
  db.addDoctor(doctor);
  // 患者添加数据测试
  BaseInfo patient_info;
  patient_info.address = "北京市海淀区";
  patient_info.id_num = "000000xx";
  patient_info.age = 30;
  patient_info.gender = 1;
  patient_info.phone_number = "5555555";
  Patient patient("李四", "pwd", patient_info);
  patient.allergens = "蛋白质";
  patient.birthday = "2000年1月1日";
  db.addPatient(patient);
  // 添加预约信息
  ReservationInfo zs_reservation;
  zs_reservation.date = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  zs_reservation.doctor_name = "张三";
  zs_reservation.patient_name = "李四";
  db.addReservation(zs_reservation);
  // 添加病例
  CaseInfo case_info;
  case_info.doctor_name = "张三";
  case_info.patient_name = "李四";
  case_info.patient_case = "感冒";
  case_info.medical_advice = "多喝水";
  case_info.prescription = "板蓝根";
  db.addCase(case_info);
  // 查询数据
  Doctor search_doctor = db.queryDoctor("张三");
  std::cout << "Doctor: " << search_doctor.username << std::endl;
  std::cout << "username: " << search_doctor.username << std::endl;
  std::cout << "pwd: " << search_doctor.pwd << std::endl;
  std::cout << "identity: " << search_doctor.identify << std::endl;
  std::cout << "title: " << search_doctor.title << std::endl;
  std::cout << "department: " << search_doctor.department << std::endl;
  std::cout << "id_num: " << search_doctor.base_info.id_num << std::endl;
  std::cout << "years_of_service: " << search_doctor.years_of_service << std::endl;
  std::cout << "age: " << search_doctor.base_info.age << std::endl;
  std::cout << "gender: " << search_doctor.base_info.gender << std::endl;
  std::cout << "phone_number: " << search_doctor.base_info.phone_number << std::endl;
  std::cout << "address: " << search_doctor.base_info.address << std::endl;
  // 患者查询
  Patient search_patient = db.queryPatient("李四");
  std::cout << "Patient: " << search_patient.username << std::endl;
  std::cout << "username: " << search_patient.username << std::endl;
  std::cout << "pwd: " << search_patient.pwd << std::endl;
  std::cout << "identity: " << search_patient.identify << std::endl;
  std::cout << "id_num: " << search_patient.base_info.id_num << std::endl;
  std::cout << "birthday: " << search_patient.birthday << std::endl;
  std::cout << "allergens: " << search_patient.allergens << std::endl;
  std::cout << "age: " << search_patient.base_info.age << std::endl;
  std::cout << "gender: " << search_patient.base_info.gender << std::endl;
  std::cout << "phone_number: " << search_patient.base_info.phone_number << std::endl;
  std::cout << "address: " << search_patient.base_info.address << std::endl;
  int num = 0;
  std::cout << "相关病例查询：" << std::endl;
  // 同样可以通过search_patient.case_info获取患者的病例信息
  for (auto caseinfo : search_doctor.dialogs) {
    std::cout << "CaseInfo " << ++num << ":" << std::endl;
    std::cout << "doctor_id: " << caseinfo.doctor_name << std::endl;
    std::cout << "patient_id: " << caseinfo.patient_name << std::endl;
    std::cout << "patient_case: " << caseinfo.patient_case << std::endl;
    std::cout << "medical_advice: " << caseinfo.medical_advice << std::endl;
    std::cout << "prescription: " << caseinfo.prescription << std::endl;
  }
  std::cout << "预约信息查询： " << std::endl;
  num = 0;
  // 同样可以通过search_patient.reservation获取患者的预约信息
  for (auto reservation : search_doctor.reservations) {
    std::cout << "ReservationInfo " << ++num << ":" << std::endl;
    std::cout << "doctor_id: " << reservation.doctor_name << std::endl;
    std::cout << "patient_id: " << reservation.patient_name << std::endl;
    std::cout << "date: " << std::ctime(&reservation.date) << std::endl;
  }

  std::cout << "---------------------------------" << std::endl;
  // 更新数据
  Doctor doctor_update("王五", "pwd", base_info);
  doctor.title = "主任";
  doctor.department = "内科";
  doctor.years_of_service = 11;
  doctor.base_info = base_info;
  db.updateDoctor(1, doctor_update);
  // 查询数据
  search_doctor = db.queryDoctor("王五");
  std::cout << "Doctor: " << search_doctor.username << std::endl;
  std::cout << "username: " << search_doctor.username << std::endl;
  std::cout << "pwd: " << search_doctor.pwd << std::endl;
  std::cout << "identity: " << search_doctor.identify << std::endl;
  std::cout << "title: " << search_doctor.title << std::endl;
  std::cout << "department: " << search_doctor.department << std::endl;
  std::cout << "id_num: " << search_doctor.base_info.id_num << std::endl;
  std::cout << "years_of_service: " << search_doctor.years_of_service << std::endl;
  std::cout << "age: " << search_doctor.base_info.age << std::endl;
  std::cout << "gender: " << search_doctor.base_info.gender << std::endl;
  std::cout << "phone_number: " << search_doctor.base_info.phone_number << std::endl;
  std::cout << "address: " << search_doctor.base_info.address << std::endl;
  return 0;
}
