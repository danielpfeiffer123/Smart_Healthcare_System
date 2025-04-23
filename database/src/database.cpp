#include "database.h"

// 创建医生表
const std::string create_doctor_table = 
  "CREATE TABLE Doctors ("
  "doctor_id INTEGER PRIMARY KEY AUTOINCREMENT,"
  "username TEXT NOT NULL UNIQUE,"
  "password TEXT NOT NULL,"
  "title TEXT DEFAULT NULL,"
  "department TEXT DEFAULT NULL,"
  "id_num TEXT DEFAULT NULL,"
  "years_of_server INTEGER DEFAULT 0,"
  "age INTEGER DEFAULT NULL,"
  "gender TEXT DEFAULT NULL,"
  "phone_number TEXT DEFAULT NULL,"
  "address TEXT DEFAULT NULL);";
// 创建患者表
const std::string create_patient_table = 
  "CREATE TABLE Patients ("
  "patient_id INTEGER PRIMARY KEY AUTOINCREMENT,"
  "username TEXT NOT NULL UNIQUE,"
  "password TEXT NOT NULL,"
  "id_num TEXT DEFAULT NULL,"
  "birthday TEXT DEFAULT NULL,"
  "allergens TEXT DEFAULT NULL,"
  "age INTEGER DEFAULT NULL,"
  "gender TEXT DEFAULT NULL,"
  "phone_number TEXT DEFAULT NULL,"
  "address TEXT DEFAULT NULL);";
// 创建预约表
const std::string create_reservation_table = 
  "CREATE TABLE IF NOT EXISTS Reservations ("
  "reservation_id INTEGER PRIMARY KEY AUTOINCREMENT,"
  "patient_id INTEGER NOT NULL,"
  "doctor_id INTEGER NOT NULL,"
  "appointment_date TEXT NOT NULL,"
  "FOREIGN KEY (patient_id) REFERENCES Patients(patient_id),"
  "FOREIGN KEY (doctor_id) REFERENCES Doctors(doctor_id));";
// 创建病例表
const std::string create_case_table = 
  "CREATE TABLE IF NOT EXISTS Cases ("
  "case_id INTEGER PRIMARY KEY AUTOINCREMENT,"
  "patient_id INTEGER NOT NULL,"
  "doctor_id INTEGER NOT NULL,"
  "case_info TEXT NOT NULL,"
  "medical_advice TEXT,"
  "prescription TEXT,"
  "FOREIGN KEY (patient_id) REFERENCES Patients(patient_id),"
  "FOREIGN KEY (doctor_id) REFERENCES Doctors(doctor_id));";

void DataBase::executeSQL(sqlite3* db, const std::string& sql) {
  char* errMessage = nullptr;
  int exitCode = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMessage);
  if (exitCode != SQLITE_OK) {
    std::cerr << "SQL error: " << errMessage << std::endl;
    sqlite3_free(errMessage);
  }
}

void DataBase::InitDataBase() {
  // 打开数据库，如果不存在则创建
  int exitCode = sqlite3_open(db_name.c_str(), &db);
  if (exitCode) {
    std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_close(db);
  }

  // 检查表是否存在，如果不存在则创建
  this->executeSQL(this->db, create_doctor_table);
  this->executeSQL(this->db, create_patient_table);
  this->executeSQL(this->db, create_reservation_table);
  this->executeSQL(this->db, create_case_table);
}

Doctor DataBase::queryDoctor(std::string username) {
  // 医生基本信息查询
  int doctor_id = -1;
  std::string sql = "SELECT * FROM Doctors WHERE username = ?;";
  sqlite3_stmt* stmt;
  int exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return Doctor();
  }

  sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
  exitCode = sqlite3_step(stmt);
  Doctor doctor;
  if (exitCode == SQLITE_ROW) {
    doctor_id = sqlite3_column_int(stmt, 0);
    std::string username = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    std::string pwd = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    std::string title = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
    std::string department = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
    std::string id_num = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
    int years_of_server = sqlite3_column_int(stmt, 6);
    int age = sqlite3_column_int(stmt, 7);
    std::string gender = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8)));
    int ge = -1;
    if (gender == "male") {
      ge = 1;
    } else if (gender == "female") {
      ge = 0;
    }
    std::string phone_number = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9)));
    std::string address = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 10)));
    BaseInfo base_info = BaseInfo(age, ge,id_num, phone_number, address);
    doctor = Doctor(username, pwd, base_info);
    doctor.title = title;
    doctor.department = department;
    doctor.years_of_service = years_of_server;
  }

  // 查询所有与该医生相关预约信息
  sql = "SELECT * FROM Reservations WHERE doctor_id = ?;";
  exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return doctor;
  }
  sqlite3_bind_int(stmt, 1, doctor_id);
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    std::string appointment_date = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
    ReservationInfo reservation_info;
    reservation_info.patient_name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    reservation_info.doctor_name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    reservation_info.date = std::stoi(appointment_date);
    doctor.reservations.push_back(reservation_info);
  }
  // 查询所有与该医生相关的病例信息
  sql = "SELECT * FROM Cases WHERE doctor_id = ?;";
  exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return doctor;
  }
  sqlite3_bind_int(stmt, 1, doctor_id);
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    CaseInfo case_info;
    case_info.patient_name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    case_info.doctor_name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    case_info.patient_case = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
    case_info.medical_advice = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
    case_info.prescription = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
    doctor.dialogs.push_back(case_info);
  }

  sqlite3_finalize(stmt);
  return doctor;
}

Patient DataBase::queryPatient(std::string username) {
  // 患者基本信息查询
  int patient_id = -1;
  std::string sql = "SELECT * FROM Patients WHERE username = ?;";
  sqlite3_stmt* stmt;
  int exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return Patient();
  }

  sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
  exitCode = sqlite3_step(stmt);
  Patient patient;
  if (exitCode == SQLITE_ROW) {
    patient_id = sqlite3_column_int(stmt, 0);
    std::string username = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    std::string pwd = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    std::string id_num = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
    std::string birthday = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
    std::string allergens = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
    int age = sqlite3_column_int(stmt, 6);
    std::string gender = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)));
    int ge = -1;
    if (gender == "male") {
      ge = 1;
    } else if (gender == "female") {
      ge = 0;
    }
    std::string phone_number = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 8)));
    std::string address = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 9)));
    BaseInfo base_info = BaseInfo(age, ge, id_num, phone_number, address);
    patient = Patient(username, pwd, base_info);
  }

  // 查询所有与该患者相关预约信息
  sql = "SELECT * FROM Reservations WHERE patient_id = ?;";
  exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return patient;
  }
  sqlite3_bind_int(stmt, 1, patient_id);
  if (sqlite3_step(stmt) == SQLITE_ROW) {
    std::string appointment_date = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
    ReservationInfo reservation_info;
    reservation_info.patient_name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    reservation_info.doctor_name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    reservation_info.date = std::stoi(appointment_date);
    patient.reservation = reservation_info;
  }
  // 查询所有与该患者相关的病例信息
  sql = "SELECT * FROM Cases WHERE patient_id = ?;";
  exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return patient;
  }
  sqlite3_bind_int(stmt, 1, patient_id);
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    CaseInfo case_info;
    case_info.patient_name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
    case_info.doctor_name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    case_info.patient_case = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
    case_info.medical_advice = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
    case_info.prescription = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5)));
    patient.case_info.emplace_back(case_info);
  }

  sqlite3_finalize(stmt);
  return patient;
}

void DataBase::addDoctor(Doctor doctor) {
  std::string sql = "INSERT INTO Doctors (username, password, title, department, id_num, years_of_server, age, gender, phone_number, address) "
                    "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

  sqlite3_stmt* stmt;
  int exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  
  if (exitCode != SQLITE_OK) {
      std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
      return;
  }
  sqlite3_bind_text(stmt, 1, doctor.username.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, doctor.pwd.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, doctor.title.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 4, doctor.department.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 5, doctor.base_info.id_num.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 6, doctor.years_of_service);
  sqlite3_bind_int(stmt, 7, doctor.base_info.age);
  std::string gender = "unknown";
  if (doctor.base_info.gender == 1) {
    gender = "male";
  } else if (doctor.base_info.gender == 0) {
    gender = "female";
  }
  sqlite3_bind_text(stmt, 8, gender.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 9, doctor.base_info.phone_number.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 10, doctor.base_info.address.c_str(), -1, SQLITE_STATIC);

  exitCode = sqlite3_step(stmt);
  if (exitCode != SQLITE_DONE) {
    std::cerr << "Failed to insert data: " << sqlite3_errmsg(db) << std::endl;
  }
  sqlite3_finalize(stmt);
}

void DataBase::addPatient(Patient patient) {
    std::string sql = "INSERT INTO Patients (username, password, id_num, birthday, allergens, age, gender, phone_number, address) "
                    "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

  sqlite3_stmt* stmt;
  int exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  
  if (exitCode != SQLITE_OK) {
      std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
      return;
  }
  sqlite3_bind_text(stmt, 1, patient.username.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, patient.pwd.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 3, patient.base_info.id_num.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 4, patient.birthday.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 5, patient.allergens.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 6, patient.base_info.age);
  std::string gender = "unknown";
  if (patient.base_info.gender == 1) {
    gender = "male";
  } else if (patient.base_info.gender == 0) {
    gender = "female";
  }
  sqlite3_bind_text(stmt, 7, gender.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 8, patient.base_info.phone_number.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 9, patient.base_info.address.c_str(), -1, SQLITE_STATIC);

  exitCode = sqlite3_step(stmt);
  if (exitCode != SQLITE_DONE) {
    std::cerr << "Failed to insert data: " << sqlite3_errmsg(db) << std::endl;
  }
  sqlite3_finalize(stmt);
}

void DataBase::addReservation(ReservationInfo reservation) {
  std::string sql = "INSERT INTO Reservations (patient_id, doctor_id, appointment_date) "
                    "VALUES (?, ?, ?);";

  sqlite3_stmt* stmt;
  int exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  
  if (exitCode != SQLITE_OK) {
      std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
      return;
  }
  // 查询患者id
  std::string sql_query = "SELECT patient_id FROM Patients WHERE username = ?;";
  sqlite3_stmt* stmt_query;
  exitCode = sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt_query, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  sqlite3_bind_text(stmt_query, 1, reservation.patient_name.c_str(), -1, SQLITE_STATIC);
  exitCode = sqlite3_step(stmt_query);
  int patient_id = -1;
  if (exitCode == SQLITE_ROW) {
    patient_id = sqlite3_column_int(stmt_query, 0);
  }
  sqlite3_finalize(stmt_query);

  // 查询医生id
  sql_query = "SELECT doctor_id FROM Doctors WHERE username = ?;";
  exitCode = sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt_query, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  sqlite3_bind_text(stmt_query, 1, reservation.doctor_name.c_str(), -1, SQLITE_STATIC);
  exitCode = sqlite3_step(stmt_query);
  int doctor_id = -1;
  if (exitCode == SQLITE_ROW) {
    doctor_id = sqlite3_column_int(stmt_query, 0);
  }
  sqlite3_finalize(stmt_query);

  sqlite3_bind_int(stmt, 1, patient_id);
  sqlite3_bind_int(stmt, 2, doctor_id);
  sqlite3_bind_text(stmt, 3, std::to_string(reservation.date).c_str(), -1, SQLITE_STATIC);

  exitCode = sqlite3_step(stmt);
  if (exitCode != SQLITE_DONE) {
    std::cerr << "Failed to insert data: " << sqlite3_errmsg(db) << std::endl;
  }
  sqlite3_finalize(stmt);
}

void DataBase::addCase(CaseInfo caseinfo) {
  std::string sql = "INSERT INTO Cases (patient_id, doctor_id, case_info, medical_advice, prescription) "
                    "VALUES (?, ?, ?, ?, ?);";
  
  sqlite3_stmt* stmt;
  int exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  // 查询患者id
  std::string sql_query = "SELECT patient_id FROM Patients WHERE username = ?;";
  sqlite3_stmt* stmt_query;
  exitCode = sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt_query, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  sqlite3_bind_text(stmt_query, 1, caseinfo.patient_name.c_str(), -1, SQLITE_STATIC);
  exitCode = sqlite3_step(stmt_query);
  int patient_id = -1;
  if (exitCode == SQLITE_ROW) {
    patient_id = sqlite3_column_int(stmt_query, 0);
  }
  sqlite3_finalize(stmt_query);

  // 查询医生id
  sql_query = "SELECT doctor_id FROM Doctors WHERE username = ?;";
  exitCode = sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt_query, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  sqlite3_bind_text(stmt_query, 1, caseinfo.doctor_name.c_str(), -1, SQLITE_STATIC);
  exitCode = sqlite3_step(stmt_query);
  int doctor_id = -1;
  if (exitCode == SQLITE_ROW) {
    doctor_id = sqlite3_column_int(stmt_query, 0);
  }
  sqlite3_finalize(stmt_query);

  sqlite3_bind_int(stmt, 1, patient_id);
  sqlite3_bind_int(stmt, 2, doctor_id);
  sqlite3_bind_text(stmt, 3, caseinfo.patient_case.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 4, caseinfo.medical_advice.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 5, caseinfo.prescription.c_str(), -1, SQLITE_STATIC);

  exitCode = sqlite3_step(stmt);
  if (exitCode != SQLITE_DONE) {
    std::cerr << "Failed to insert data: " << sqlite3_errmsg(db) << std::endl;
  }
  sqlite3_finalize(stmt);
}

void DataBase::deleteDoctor(std::string username) {
  std::string sql = "DELETE FROM Doctors WHERE username = ?;";
  sqlite3_stmt* stmt;
  int exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
  exitCode = sqlite3_step(stmt);
  if (exitCode != SQLITE_DONE) {
    std::cerr << "Failed to delete data: " << sqlite3_errmsg(db) << std::endl;
  }
  sqlite3_finalize(stmt);
}

void DataBase::deletePatient(std::string username) {
  std::string sql = "DELETE FROM Patients WHERE username = ?;";
  sqlite3_stmt* stmt;
  int exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
  exitCode = sqlite3_step(stmt);
  if (exitCode != SQLITE_DONE) {
    std::cerr << "Failed to delete data: " << sqlite3_errmsg(db) << std::endl;
  }
  sqlite3_finalize(stmt);
}

void DataBase::deleteReservation(int reservation_id) {
  std::string sql = "DELETE FROM Reservations WHERE reservation_id = ?;";
  sqlite3_stmt* stmt;
  int exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  sqlite3_bind_int(stmt, 1, reservation_id);
  exitCode = sqlite3_step(stmt);
  if (exitCode != SQLITE_DONE) {
    std::cerr << "Failed to delete data: " << sqlite3_errmsg(db) << std::endl;
  }
  sqlite3_finalize(stmt);
}

void DataBase::deleteCase(int case_id) {
  std::string sql = "DELETE FROM Cases WHERE case_id = ?;";
  sqlite3_stmt* stmt;
  int exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  sqlite3_bind_int(stmt, 1, case_id);
  exitCode = sqlite3_step(stmt);
  if (exitCode != SQLITE_DONE) {
    std::cerr << "Failed to delete data: " << sqlite3_errmsg(db) << std::endl;
  }
  sqlite3_finalize(stmt);
}

void DataBase::updateDoctor(int doctor_id, Doctor doctor) {
  // 通过doctor_id更新医生信息
  std::string sql =
    "UPDATE Doctors"
    " SET username = ?, password = ?, age = ?, gender = ?, phone_number = ?, address = ?, title = ?, department = ?, id_num = ?, years_of_server = ?"
    " WHERE doctor_id = ?;";
  sqlite3_stmt* stmt;
  int exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  sqlite3_bind_text(stmt, 1, doctor.username.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, doctor.pwd.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 3, doctor.base_info.age);
  std::string gender = "unknown";
  if (doctor.base_info.gender == 1) {
    gender = "male";
  } else if (doctor.base_info.gender == 0) {
    gender = "female";
  }
  sqlite3_bind_text(stmt, 4, gender.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 5, doctor.base_info.phone_number.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 6, doctor.base_info.address.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 7, doctor.title.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 8, doctor.department.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 9, doctor.base_info.id_num.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 10, doctor.years_of_service);
  sqlite3_bind_int(stmt, 11, doctor_id);
  
  exitCode = sqlite3_step(stmt);
  if (exitCode != SQLITE_DONE) {
    std::cerr << "Failed to update data: " << sqlite3_errmsg(db) << std::endl;
  }
  sqlite3_finalize(stmt);
}

void DataBase::updatePatient(int patient_id, Patient patient) {
  // 通过patient_id更新患者信息
  std::string sql =
    "UPDATE Patients"
    " SET username = ?, password = ?, age = ?, gender = ?, phone_number = ?, address = ?, id_num = ?, birthday = ?, allergens = ?"
    " WHERE patient_id = ?;";
  sqlite3_stmt* stmt;
  int exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  sqlite3_bind_text(stmt, 1, patient.username.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 2, patient.pwd.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 3, patient.base_info.age);
    std::string gender = "unknown";
  if (patient.base_info.gender == 1) {
    gender = "male";
  } else if (patient.base_info.gender == 0) {
    gender = "female";
  }
  sqlite3_bind_text(stmt, 4, patient.base_info.phone_number.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 5, patient.base_info.address.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 6, patient.base_info.id_num.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 7, patient.birthday.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 8, patient.allergens.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 9, patient_id);

  exitCode = sqlite3_step(stmt);
  if (exitCode != SQLITE_DONE) {
    std::cerr << "Failed to update data: " << sqlite3_errmsg(db) << std::endl;
  }
  sqlite3_finalize(stmt);
}

void DataBase::updateReservation(int reservation_id, ReservationInfo reservation) {
  // 通过reservation_id更新预约信息
  std::string sql =
    "UPDATE Reservations"
    " SET patient_id = ?, doctor_id = ?, appointment_date = ?"
    " WHERE reservation_id = ?;";
  sqlite3_stmt* stmt;
  int exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  // 查询患者id
  std::string sql_query = "SELECT patient_id FROM Patients WHERE username = ?;";
  sqlite3_stmt* stmt_query;
  exitCode = sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt_query, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  sqlite3_bind_text(stmt_query, 1, reservation.patient_name.c_str(), -1, SQLITE_STATIC);
  exitCode = sqlite3_step(stmt_query);
  int patient_id = -1;
  if (exitCode == SQLITE_ROW) {
    patient_id = sqlite3_column_int(stmt_query, 0);
  }
  sqlite3_finalize(stmt_query);

  // 查询医生id
  sql_query = "SELECT doctor_id FROM Doctors WHERE username = ?;";
  exitCode = sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt_query, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  sqlite3_bind_text(stmt_query, 1, reservation.doctor_name.c_str(), -1, SQLITE_STATIC);
  exitCode = sqlite3_step(stmt_query);
  int doctor_id = -1;
  if (exitCode == SQLITE_ROW) {
    doctor_id = sqlite3_column_int(stmt_query, 0);
  }
  sqlite3_finalize(stmt_query);

  sqlite3_bind_int(stmt, 1, patient_id);
  sqlite3_bind_int(stmt, 2, doctor_id);
  sqlite3_bind_text(stmt, 3, std::to_string(reservation.date).c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 4, reservation_id);

  exitCode = sqlite3_step(stmt);
  if (exitCode != SQLITE_DONE) {
    std::cerr << "Failed to update data: " << sqlite3_errmsg(db) << std::endl;
  }
  sqlite3_finalize(stmt);
}

void DataBase::updateCase(int case_id, CaseInfo caseinfo) {
  // 通过case_id更新病例信息
  std::string sql =
    "UPDATE Cases"
    " SET patient_id = ?, doctor_id = ?, case_info = ?, medical_advice = ?, prescription = ?"
    " WHERE case_id = ?;";
  sqlite3_stmt* stmt;
  int exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  // 查询患者id
  std::string sql_query = "SELECT patient_id FROM Patients WHERE username = ?;";
  sqlite3_stmt* stmt_query;
  exitCode = sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt_query, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  sqlite3_bind_text(stmt_query, 1, caseinfo.patient_name.c_str(), -1, SQLITE_STATIC);
  exitCode = sqlite3_step(stmt_query);
  int patient_id = -1;
  if (exitCode == SQLITE_ROW) {
    patient_id = sqlite3_column_int(stmt_query, 0);
  }
  sqlite3_finalize(stmt_query);

  // 查询医生id
  sql_query = "SELECT doctor_id FROM Doctors WHERE username = ?;";
  exitCode = sqlite3_prepare_v2(db, sql_query.c_str(), -1, &stmt_query, nullptr);
  if (exitCode != SQLITE_OK) {
    std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
    return;
  }
  sqlite3_bind_text(stmt_query, 1, caseinfo.doctor_name.c_str(), -1, SQLITE_STATIC);
  exitCode = sqlite3_step(stmt_query);
  int doctor_id = -1;
  if (exitCode == SQLITE_ROW) {
    doctor_id = sqlite3_column_int(stmt_query, 0);
  }
  sqlite3_finalize(stmt_query);

  sqlite3_bind_int(stmt, 1, patient_id);
  sqlite3_bind_int(stmt, 2, doctor_id);
  sqlite3_bind_text(stmt, 3, caseinfo.patient_case.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 4, caseinfo.medical_advice.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_text(stmt, 5, caseinfo.prescription.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 6, case_id);

  exitCode = sqlite3_step(stmt);
  if (exitCode != SQLITE_DONE) {
    std::cerr << "Failed to update data: " << sqlite3_errmsg(db) << std::endl;
  }
  sqlite3_finalize(stmt);
}