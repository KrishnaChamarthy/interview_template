#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

struct Entity {
    string id;
};

Entity parse_entity(const json& j) {
    return Entity{
        j.at("id").get<string>() 
    };
}

void process_api(const string& base_url) {
    string starting_after = "";
    bool has_more = true;

    while (has_more) {
        cpr::Parameters params;
        if (!starting_after.empty()) {
            params.Add({"starting_after", starting_after});
        }

        cpr::Response r = cpr::Get(cpr::Url{base_url}, params);

        if (r.status_code != 200) {
            cerr << "[HTTP Error]: " << r.status_code << " - " << r.error.message << "\n";
            break;
        }

        try {
            json payload = json::parse(r.text);
            
            if (!payload.contains("data") || !payload["data"].is_array()) {
                cerr << "[Schema Error]: Missing 'data' array.\n";
                break;
            }

            for (const auto& item : payload["data"]) {
                Entity e = parse_entity(item);
            }

            has_more = payload.value("has_more", false);
            if (has_more && payload.contains("next_page_token")) {
                starting_after = payload.at("next_page_token").get<string>();
            } else {
                has_more = false;
            }

        } catch (const json::exception& e) {
            cerr << "[JSON Parse Error]: " << e.what() << "\n";
            break; 
        }
    }
}

int main() {
    string api_url = "http://localhost:8080"; 
    cout << "Starting application...\n";
    
    process_api(api_url);
    
    cout << "Processing complete.\n";
    return 0;
}
