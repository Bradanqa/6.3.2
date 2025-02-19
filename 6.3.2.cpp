#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <fstream>


   class Observer {
   public:
      virtual ~Observer() = default;
      virtual void onWarning(const std::string& message) {}
      virtual void onError(const std::string& message) {}
      virtual void onFatalError(const std::string& message) {}
   };

   class Observed {
   public:
      void addObserver(Observer* observer) {
         observers.push_back(observer);
      }

      void removeObserver(Observer* observer) {
         auto it = std::remove(observers.begin(), observers.end(), observer);
         observers.erase(it, observers.end());
      }

   protected:
      void notifyWarning(const std::string& message) {
         for (auto* observer : observers) {
            observer->onWarning(message);
         }
      }

      void notifyError(const std::string& message) {
         for (auto* observer : observers) {
            observer->onError(message);
         }
      }

      void notifyFatalError(const std::string& message) {
         for (auto* observer : observers) {
            observer->onFatalError(message);
         }
      }

   private:
      std::vector<Observer*> observers;
   };

   class WarningObs : public Observer {
   public:
      void onWarning(const std::string& message) override {
         std::cout << "Warning to console: " << message << std::endl;
      }
   };

   class ErrorObs : public Observer {
   public:
      ErrorObs(const std::string& file_name) {
         fileName = file_name;
      }

      void onError(const std::string& message) override {
         std::ofstream fileStream(fileName, std::ios::app);
         if (!fileStream.is_open()) {
            throw std::runtime_error("File is not open");
         }

         fileStream << "Error to log file: " << message << std::endl;
         fileStream.close();
      }

   private:
      std::string fileName;
   };

   class FatalObs : public Observer {
   public:
      FatalObs(const std::string& file_name) {
         fileName = file_name;
      }

      void onFatalError(const std::string& message) override {
         std::ofstream fileStream(fileName, std::ios::app);
         if (!fileStream.is_open()) {
            throw std::runtime_error("File is not open");
         }

         std::cout << "Fatal error to console: " << message << std::endl;

         fileStream << "Fatal error to log file: " << message << std::endl;
         fileStream.close();
      }

   private:
      std::string fileName;
   };


   class TestClass : public Observed {
   public:
      void job() {
         warning("warning message");
         error("error message");
         fatalError("fatal error message");
      }

      void warning(const std::string& message) {
         notifyWarning(message);
      }

      void error(const std::string& message) {
         notifyError(message);
      }

      void fatalError(const std::string& message) {
         notifyFatalError(message);
      }
   };


   int main() {
      TestClass testClass;

      WarningObs warningObserver;
      ErrorObs errorObserver("errors.log");
      FatalObs fatalErrorObserver("fatal_errors.log");

      testClass.addObserver(&warningObserver);
      testClass.addObserver(&errorObserver);
      testClass.addObserver(&fatalErrorObserver);

      testClass.job();

      testClass.removeObserver(&warningObserver);
      testClass.removeObserver(&errorObserver);
      testClass.removeObserver(&fatalErrorObserver);

      return 0;
   }
