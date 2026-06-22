#ifndef TASK_CPP
#define TASK_CPP

#include<string>
#include<chrono>
#include<stdexcept>

#define STATUS_TODO 0
#define STATUS_IN_PROGRESS 1
#define STATUS_DONE 2

class Task {
    public:
        Task(int id, const std::string& description) : id(id), description(description), status(STATUS_TODO) {
            created_at = std::chrono::system_clock::now();
            last_updated_at = created_at;
        }
        
        // Getters
        int getId() const { return id; }
        std::string getDescription() const { return description; }
        int getStatus() const { return status; }

        // Setters (both refresh last_updated_at)
        void setDescription(const std::string& newDescription) {
            description = newDescription;
            last_updated_at = std::chrono::system_clock::now();
        }
        void setStatus(int newStatus) {
            status = newStatus;
            last_updated_at = std::chrono::system_clock::now();
        }

        std::string toJsonStringFormat() const {
            
            std::string jsonString = "{";
            jsonString += "\"id\": " + std::to_string(id) + ",";
            jsonString += "\"description\": \"" + description + "\",";
            jsonString += "\"status\": " + std::to_string(status) + ",";
            jsonString += "\"created_at\": \"" + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(created_at.time_since_epoch()).count()) + "\",";
            jsonString += "\"last_updated_at\": \"" + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(last_updated_at.time_since_epoch()).count()) + "\"";
            jsonString += "}";
            
            return jsonString;
        }

        static Task stringToTask(const std::string& jsonString) {
            auto extractValue = [&](const std::string& key) -> std::string {
                std::string searchKey = "\"" + key + "\":";
                size_t keyPos = jsonString.find(searchKey);
                if (keyPos == std::string::npos) {
                    throw std::invalid_argument("Key not found: " + key);
                }
                size_t valueStart = keyPos + searchKey.size();

                // Skip optional whitespace after ':'
                while (valueStart < jsonString.size() && jsonString[valueStart] == ' ') {
                    ++valueStart;
                }

                std::string raw;
                if (jsonString[valueStart] == '"') {
                    // Quoted string value — find the closing quote (handles no escape sequences)
                    ++valueStart; // skip opening quote
                    size_t valueEnd = jsonString.find('"', valueStart);
                    if (valueEnd == std::string::npos) {
                        throw std::invalid_argument("Unterminated string for key: " + key);
                    }
                    raw = jsonString.substr(valueStart, valueEnd - valueStart);
                } else {
                    // Numeric value — ends at ',' or '}'
                    size_t valueEnd = jsonString.find_first_of(",}", valueStart);
                    if (valueEnd == std::string::npos) {
                        valueEnd = jsonString.size();
                    }
                    raw = jsonString.substr(valueStart, valueEnd - valueStart);
                }
                return raw;
            };

            int parsedId          = std::stoi(extractValue("id"));
            std::string parsedDesc = extractValue("description");
            int parsedStatus      = std::stoi(extractValue("status"));
            long long parsedCreatedAt     = std::stoll(extractValue("created_at"));
            long long parsedLastUpdatedAt = std::stoll(extractValue("last_updated_at"));

            // Build time_points from stored epoch-seconds values
            std::chrono::system_clock::time_point createdAtTp =
                std::chrono::system_clock::time_point(std::chrono::seconds(parsedCreatedAt));
            std::chrono::system_clock::time_point lastUpdatedAtTp =
                std::chrono::system_clock::time_point(std::chrono::seconds(parsedLastUpdatedAt));

            return Task(parsedId, parsedDesc, parsedStatus, createdAtTp, lastUpdatedAtTp);
        }

    private: 
        int id;
        std::string description;
        int status;
        std::chrono::system_clock::time_point created_at;
        std::chrono::system_clock::time_point last_updated_at;

        // Private full constructor used by stringToTask() to restore all fields from storage.
        Task(int id, const std::string& description, int status,
             std::chrono::system_clock::time_point created_at,
             std::chrono::system_clock::time_point last_updated_at)
            : id(id), description(description), status(status),
              created_at(created_at), last_updated_at(last_updated_at) {}
};

#endif // TASK_CPP