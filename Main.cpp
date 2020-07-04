# include <Siv3D.hpp> // OpenSiv3D v0.4.3


struct Meigen {
    int32 id;
    String author;
    String content;
};


Array<Meigen> meigensLoad(JSONReader meigens_json) {
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

    return meigens;
}


std::pair<Array<std::pair<DrawableText, Rect>>, Size> makePack(const Array<Meigen>& meigens) {
    const Array<Font> fonts = {
        Font(20),
        Font(30),
        Font(40)
    };

    Array<DrawableText> drawable_texts;
    Array<Rect> content_rects;
    for (const auto& meigen : meigens) {
        DrawableText d_text = fonts.choice()(meigen.content);
        drawable_texts << d_text;
        content_rects << d_text.region();
    }

    std::pair<Array<Rect>, Size> pack = RectanglePacking::Pack(content_rects, 1200);
    Array<std::pair<DrawableText, Rect>> texts;
    for (auto [i, rect] : Indexed(pack.first)) {
        texts << std::make_pair(
            drawable_texts[i],
            rect
        );
    }

    return std::make_pair(
        texts,
        pack.second
    );
}


void Main() {
    Window::Resize(1600, 900);
    Scene::SetBackground(Palette::White);

    const JSONReader meigens_json(U"meigens.json");
    const Array<Meigen> meigens = meigensLoad(meigens_json);

    std::pair<Array<std::pair<DrawableText, Rect>>, Size> pack;
    do {
        const Array<Meigen> selected_meigens = meigens.choice(20);
        pack = makePack(selected_meigens);
    } while (pack.second.isZero());


    const Array<std::pair<DrawableText, Rect>> texts = pack.first;
    const Size size = pack.second;

    while (System::Update()) {
        Rect(size).drawFrame(0.0, 1.0, Palette::Black);

        for (const auto& text : texts) {
            text.first.draw(text.second, Palette::Black);
        }

        if (KeySpace.down()) {
            ScreenCapture::RequestCurrentFrame();
        }

        if (ScreenCapture::HasNewFrame()) {
            const Image clipped_screen = ScreenCapture::GetFrame().clipped(Rect(size));
            clipped_screen.savePNG(U"meigen.png");
            Clipboard::SetImage(clipped_screen);
        }
    }
}
