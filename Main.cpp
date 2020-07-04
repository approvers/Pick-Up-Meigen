# include <Siv3D.hpp> // OpenSiv3D v0.4.3

struct Meigen {
    int32 id;
    String author;
    String content;
};

void Main() {
    Window::Resize(1600, 900);
    Scene::SetBackground(Palette::White);

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


    const Array<Font> fonts = {
        Font(20),
        Font(30),
        Font(40)
    };

    Array<Meigen> selected_meigens;
    Array<DrawableText> drawable_texts;
    Array<Rect> content_rects;
    std::pair<Array<Rect>, Size> pack;

    do {
        selected_meigens = meigens.choice(20);
        for (const auto& meigen : selected_meigens) {
            DrawableText d_text = fonts.choice()(meigen.content);
            drawable_texts << d_text;
            content_rects << d_text.region();
        }
        pack = RectanglePacking::Pack(content_rects, 1200);
    } while (pack.first.empty());

    while (System::Update()) {
        Rect(pack.second).drawFrame(0.0, 1.0, Palette::Black);

        for (auto [i, rect] : Indexed(pack.first)) {
            drawable_texts[i].draw(rect.pos, Palette::Black);
        }

        if (KeySpace.down()) {
            ScreenCapture::RequestCurrentFrame();
        }
    }
}
