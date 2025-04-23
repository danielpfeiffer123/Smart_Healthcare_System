#include <sqlite3.h>
#include <iostream>
#include <string>
#include <vector>

#include "model.h"

const std::string db_name = "./hospital.db";

class DataBase {
  public:
    DataBase() {InitDataBase();};
    
    void InitDataBase();
    void executeSQL(sqlite3* db, const std::string& sql);

    // 查
    Doctor queryDoctor(std::string username);
    Patient queryPatient(std::string username);

    // 增
    void addDoctor(Doctor doctor);
    void addPatient(Patient patient);
    void addReservation(ReservationInfo reservation);
    void addCase(CaseInfo case_info);

    // 改
    void updateDoctor(int doctor_id, Doctor doctor);
    void updatePatient(int patient_id, Patient patient);
    void updateReservation(int reservation_id, ReservationInfo reservation);
    void updateCase(int case_id, CaseInfo case_info);

    // 删
    void deleteDoctor(std::string username);
    void deletePatient(std::string username);
    void deleteReservation(int reservation_id);
    void deleteCase(int case_id);

  private:
    sqlite3* db;
};