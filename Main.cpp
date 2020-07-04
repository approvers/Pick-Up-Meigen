# include <Siv3D.hpp> // OpenSiv3D v0.4.3

struct Meigen {
    int32 id;
    String author;
    String content;
};


void Main() {
    const JSONReader meigens_json(U"meigens.json");

    if (!meigens_json) {
        throw Error(U"Failed to load `meigens.json`");
    }

    Array<Meigen> meigens;
    for (const auto& object : meigens_json.objectView()) {
        if (object.name != U"meigens") continue;

        for (const auto& meigen_obj : object.value.arrayView()) {
            Meigen meigen;

            meigen.id = meigen_obj[U"id"].get<int32>();
            meigen.author = meigen_obj[U"author"].getString();
            meigen.content = meigen_obj[U"content"].getString();

            meigens << meigen;
        }
    }

    while (System::Update()) {
        ClearPrint();

        for (const auto& meigen : meigens) {
            Print << U"{";
            Print << U"\t id : {}"_fmt(meigen.id);
            Print << U"\t id : {}"_fmt(meigen.author);
            Print << U"\t id : {}"_fmt(meigen.content);
            Print << U"}";
        }
    }
}
