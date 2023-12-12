//Сначала создадим структуру CountryContext, в которой будем хранить все необходимые параметры для создания объекта City :
struct CountryContext {
    string country_name;
    string country_iso_code;
    string country_phone_code;
    string country_time_zone;
    vector<Language> languages;
};

//Затем обновим ParseCitySubjson, чтобы она принимала CountryContext вместо всех отдельных параметров :
void ParseCitySubjson(vector<City>& cities, const Json& city_list_json, const CountryContext& context) {
    for (const auto& city_json : city_list_json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        cities.emplace_back(city_obj["name"s].AsString(),
            city_obj["iso_code"s].AsString(),
            context.country_phone_code + city_obj["phone_code"s].AsString(),
            context.country_name,
            context.country_iso_code,
            context.country_time_zone,
            context.languages);
    }
}

//Теперь обновим вызов ParseCitySubjson внутри ParseCountryJson :
void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        Country country = {
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString(),
        };
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
        countries.push_back(country);

        CountryContext context = {
            country.name,
            country.iso_code,
            country.phone_code,
            country.time_zone,
            country.languages
        };
        ParseCitySubjson(cities, country_obj["cities"s], context);
    }
}

//Теперь ParseCitySubjson и ParseCountryJson чище, и ParseCitySubjson стала более тестируемой, так как её можно вызвать с любым CountryContext, что облегчает создание, например, тестов.
//Кроме того, если появятся новые параметры, связанные со странами, их можно будет легко добавить в CountryContext без изменения сигнатуры функции.
//Это также улучшает расширяемость и поддерживаемость.
