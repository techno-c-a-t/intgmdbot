/**
 * intgmdbot - High-Performance C++20 Telegram GFM Bot Landing Script
 * Supports i18n (RU, BE, EN), automatic browser lang detection,
 * Theme switching (Dark/Light), and Interactive GFM Demo controller.
 */

const i18n = {
    ru: {
        docTitle: "intgmdbot — C++20 Telegram GFM Engine",
        navFeatures: "Возможности",
        navArchitecture: "Архитектура",
        navAuthor: "Об авторе",
        heroBadge: "High-Performance · C++20 · Bot API 10.1",
        heroTitle: "intgmdbot — GFM Telegram Engine",
        heroSubtitle: "Высокопроизводительный телеграм-бот на C++20 с поддержкой Telegram Bot API 10.1 Rich Messages, балансировкой GFM тегов, аккумулятором личных сообщений и 3-уровневой системой фолбеков.",
        btnGitHub: "Исходный код на GitHub",
        btnDocs: "Документация C++",
        
        // Demo Tabs
        tabRich: "Rich Message (API 10.1)",
        tabBalance: "Автобалансировка тегов",
        tabAccumulator: "Аккумулятор ЛС",
        tabFallback: "3-уровневый Фолбек",

        demoRichDesc: "Формирование нативного JSON-пейлоада <code>{\"markdown\": sanitized}</code> с полной поддержкой таблиц, списков и кодовых блоков GFM.",
        demoBalanceDesc: "Функция <code>balance_markdown()</code> автоматически закрывает незавершенные теги <code>```</code>, <code>`</code>, <code>**</code> и <code>~~</code>, исключая ошибки <code>RICH_MESSAGE_MARKDOWN_INVALID</code>.",
        demoAccumulatorDesc: "Модуль <code>MessageAccumulator</code> собирает несколько быстро отправленных сообщений пользователя в одно в пределах тайм-аута, экономя лимиты Telegram.",
        demoFallbackDesc: "Если Rich Message отклоняется сервером Telegram, движок последовательно выполняет фолбек на парсер <code>MarkdownV2 AST</code>, а затем на чистый текст.",

        demoStatus: "Статус: активен 🟢",
        demoSpeed: "Задержка: < 1 мс",

        // Features Section
        featuresTag: "Возможности",
        featuresTitle: "Почему intgmdbot?",
        fTitle1: "Bot API 10.1 Rich Messages",
        fDesc1: "Прямая отправка нативного GFM-размеченного текста без ограничений устаревшего формата MarkdownV2.",
        fTitle2: "Авто-балансировка тегов",
        fDesc2: "Интеллектуальный алгоритм закрытия парных синтаксических конструкций исключает падения из-за битого Markdown.",
        fTitle3: "Санитайзер NBSP",
        fDesc3: "Автоматическая замена неразрывных пробелов (0xC2 0xA0) на обычные для корректного отображения списков GFM.",
        fTitle4: "Аккумулятор сообщений",
        fDesc4: "Буферизация потока коротких сообщений от пользователя и их объединение в один красивый форматированный пост.",
        fTitle5: "3-уровневая надежность",
        fDesc5: "Каскадная отправка: Telegram Rich Message → MarkdownV2 AST Parser → Plain Text. Сообщение гарантированно дойдет.",
        fTitle6: "C++20 Runtime",
        fDesc6: "Сборка на CMake 3.20+, OpenSSL, cpp-httplib и nlohmann_json. Минимальное потребление RAM и мгновенный отклик.",

        // Architecture Section
        archTag: "Архитектура",
        archTitle: "Устройство движка C++20",
        archP1Title: "Message Accumulator Pipeline",
        archP1Desc: "Асинхронная очередь сообщений с фоновым потоком таймера. Объединяет последовательные сообщения от одного пользователя в единый буфер перед отправкой.",
        archP2Title: "AST-Парсер MarkdownV2",
        archP2Desc: "Собственный AST парсер для транспиляции GFM в безопасные escape-последовательности MarkdownV2 на случай недоступности Rich Message API.",
        archP3Title: "Ротируемое логирование",
        archP3Desc: "Модуль logger.cpp ведает записью события с уровнями INFO, WARNING, ERROR и CRITICAL в файл bot.log.",
        archP4Title: "Конфигурируемый JSON",
        archP4Desc: "Гибкая настройка через config.json: таймауты аккумуляции, токены и режимы парсинга без пересборки бинарного файла.",

        // Author Section
        authorTag: "Создатель",
        authorTitle: "Об авторе проекта",
        authorName: "Nikitos (@techno_c_a_t)",
        authorBio: "Студент ПМИ, создатель intgmdbot, NSLDAI, SwipeBeat и других решений на C++, Python, Linux и IoT.",
        authorBtn: "Перейти в Портфолио"
    },

    be: {
        docTitle: "intgmdbot — C++20 Telegram GFM Engine",
        navFeatures: "Магчымасці",
        navArchitecture: "Архітэктура",
        navAuthor: "Пра аўтара",
        heroBadge: "High-Performance · C++20 · Bot API 10.1",
        heroTitle: "intgmdbot — GFM Telegram Engine",
        heroSubtitle: "Высакапрадукцыйны тэлеграм-бот на C++20 з падтрымкай Telegram Bot API 10.1 Rich Messages, балансіроўкай GFM тэгаў, акумулятарам асабістых паведамленняў і 3-узроўневай сістэмай фолбекаў.",
        btnGitHub: "Выходны код на GitHub",
        btnDocs: "Дакументацыя C++",
        
        tabRich: "Rich Message (API 10.1)",
        tabBalance: "Аўтабалансіроўка тэгаў",
        tabAccumulator: "Акумулятар ЛП",
        tabFallback: "3-узроўневы Фолбек",

        demoRichDesc: "Фармаванне натыўнага JSON-пейлоада <code>{\"markdown\": sanitized}</code> з поўнай падтрымкай табліц, спісаў і кодавых блокаў GFM.",
        demoBalanceDesc: "Функцыя <code>balance_markdown()</code> аўтаматычна зачыняе незавершаныя тэгі <code>```</code>, <code>`</code>, <code>**</code> і <code>~~</code>.",
        demoAccumulatorDesc: "Модуль <code>MessageAccumulator</code> збірае некалькі хутка адпраўленых паведамленняў у адно ў межах таймаўту.",
        demoFallbackDesc: "Калі Rich Message адхіляецца серверам Telegram, рухавік паслядоўна выконвае фолбек на парсер <code>MarkdownV2 AST</code>.",

        demoStatus: "Статус: актыўны 🟢",
        demoSpeed: "Затрымка: < 1 мс",

        featuresTag: "Магчымасці",
        featuresTitle: "Чаму intgmdbot?",
        fTitle1: "Bot API 10.1 Rich Messages",
        fDesc1: "Прамая адпраўка натыўнага GFM-размечанага тэксту без абмежаванняў састарэлага фармату MarkdownV2.",
        fTitle2: "Аўта-балансіроўка тэгаў",
        fDesc2: "Інтэлектуальны алгарытм закрыцця парных сінтаксічных канструкцый выключае памылкі.",
        fTitle3: "Санітайзер NBSP",
        fDesc3: "Аўтаматычная замена неразрыўных прабелаў (0xC2 0xA0) на звычайныя для карэктнага адлюстравання спісаў GFM.",
        fTitle4: "Акумулятар паведамленняў",
        fDesc4: "Буферызацыя патоку кароткіх паведамленняў ад карыстальніка і іх аб'яднанне ў адзін прыгожы пост.",
        fTitle5: "3-узроўневая надзейнасць",
        fDesc5: "Каскадная адпраўка: Telegram Rich Message → MarkdownV2 AST Parser → Plain Text.",
        fTitle6: "C++20 Runtime",
        fDesc6: "Зборка на CMake 3.20+, OpenSSL, cpp-httplib і nlohmann_json. Мінімальнае спажыванне RAM і імгненны водгук.",

        archTag: "Архітэктура",
        archTitle: "Уладкаванне рухавіка C++20",
        archP1Title: "Message Accumulator Pipeline",
        archP1Desc: "Асінхронная чарга паведамленняў з файлавым патокам таймера. Аб'ядноўвае паслядоўныя паведамленні ў адзін буфер.",
        archP2Title: "AST-Парсер MarkdownV2",
        archP2Desc: "Уласны AST парсер для транспіляцыі GFM у бяспечныя escape-паслядоўнасці MarkdownV2.",
        archP3Title: "Ратуемае лагіраванне",
        archP3Desc: "Модуль logger.cpp вядзе запіс падзей з узроўнямі INFO, WARNING, ERROR і CRITICAL у файл bot.log.",
        archP4Title: "Канфігуруемы JSON",
        archP4Desc: "Гнуткая настройка праз config.json: таймаўты акумуляцыі, токены і рэжымы парсінгу.",

        authorTag: "Стваральнік",
        authorTitle: "Пра аўтара праекта",
        authorName: "Nikitos (@techno_c_a_t)",
        authorBio: "Студэнт ПМІ, стваральнік intgmdbot, NSLDAI, SwipeBeat і іншых рашэнняў на C++, Python, Linux і IoT.",
        authorBtn: "Перайсці ў Партфоліо"
    },

    en: {
        docTitle: "intgmdbot — C++20 Telegram GFM Engine",
        navFeatures: "Features",
        navArchitecture: "Architecture",
        navAuthor: "Author",
        heroBadge: "High-Performance · C++20 · Bot API 10.1",
        heroTitle: "intgmdbot — GFM Telegram Engine",
        heroSubtitle: "High-performance C++20 Telegram Bot with native Telegram Bot API 10.1 Rich Messages, auto GFM tag balancing, DM message accumulator, and 3-tier fallback engine.",
        btnGitHub: "GitHub Source Code",
        btnDocs: "C++ Documentation",
        
        tabRich: "Rich Message (API 10.1)",
        tabBalance: "Tag Balancer",
        tabAccumulator: "DM Accumulator",
        tabFallback: "3-Tier Fallback",

        demoRichDesc: "Builds native JSON payload <code>{\"markdown\": sanitized}</code> with full GFM tables, lists, and code block support.",
        demoBalanceDesc: "The <code>balance_markdown()</code> algorithm automatically closes unclosed <code>```</code>, <code>`</code>, <code>**</code>, and <code>~~</code> tags to prevent <code>RICH_MESSAGE_MARKDOWN_INVALID</code> errors.",
        demoAccumulatorDesc: "The <code>MessageAccumulator</code> module batches rapid consecutive DMs from a user into a single formatted post within a configurable timeout.",
        demoFallbackDesc: "If a Rich Message is rejected by Telegram API, the engine seamlessly falls back to <code>MarkdownV2 AST Parser</code> and then plain text.",

        demoStatus: "Status: Active 🟢",
        demoSpeed: "Latency: < 1 ms",

        featuresTag: "Features",
        featuresTitle: "Why intgmdbot?",
        fTitle1: "Bot API 10.1 Rich Messages",
        fDesc1: "Native GFM markup delivery without the syntax restrictions of legacy MarkdownV2.",
        fTitle2: "Auto Tag Balancing",
        fDesc2: "Smart delimiter tracking prevents bot crashes caused by malformed user Markdown syntax.",
        fTitle3: "NBSP Sanitizer",
        fDesc3: "Replaces non-breaking spaces (0xC2 0xA0) with standard spaces to ensure proper GFM list indentation.",
        fTitle4: "Message Accumulator",
        fDesc4: "Buffers rapid single-line DMs and merges them into one clean, structured rich message.",
        fTitle5: "3-Tier Reliability",
        fDesc5: "Cascading pipeline: Telegram Rich Message → MarkdownV2 AST Parser → Plain Text.",
        fTitle6: "C++20 Runtime",
        fDesc6: "Built with CMake 3.20+, OpenSSL, cpp-httplib, and nlohmann_json. Extremely low memory footprint and sub-millisecond response time.",

        archTag: "Architecture",
        archTitle: "C++20 Engine Internals",
        archP1Title: "Message Accumulator Pipeline",
        archP1Desc: "Thread-safe asynchronous message queue with background timer thread. Combines consecutive user messages into a unified buffer.",
        archP2Title: "MarkdownV2 AST Parser",
        archP2Desc: "Custom AST parser that transpiles GFM into safely escaped MarkdownV2 entities as a secondary fallback.",
        archP3Title: "Rotating File Logger",
        archP3Desc: "High-speed logger (logger.cpp) supporting INFO, WARNING, ERROR, and CRITICAL log levels outputting to bot.log.",
        archP4Title: "JSON Configuration",
        archP4Desc: "Dynamic configuration via config.json: adjust accumulation timeouts, bot tokens, and parse modes without recompiling.",

        authorTag: "Creator",
        authorTitle: "About the Author",
        authorName: "Nikitos (@techno_c_a_t)",
        authorBio: "Applied Math & CS student, creator of intgmdbot, NSLDAI, SwipeBeat, specializing in C++, Python, Linux, and IoT.",
        authorBtn: "View Portfolio"
    }
};

function detectLanguage() {
    const saved = localStorage.getItem('intgmdbot_lang');
    if (saved && i18n[saved]) return saved;

    const browserLangs = navigator.languages || [navigator.language || navigator.userLanguage];
    const isBelarusian = browserLangs.some(l => (l || '').toLowerCase().includes('be') || (l || '').toLowerCase().includes('by')) 
                      || (Intl.DateTimeFormat().resolvedOptions().timeZone || '').includes('Minsk');

    return isBelarusian ? 'be' : 'ru';
}

let currentLang = detectLanguage();

document.addEventListener('DOMContentLoaded', () => {
    // 1. Language detection using SwipeBeat timezone & locale scheme
    setLanguage(detectLanguage());

    // 2. Theme persistence
    const savedTheme = localStorage.getItem('intgmdbot_theme') || 'dark';
    setTheme(savedTheme);

    // 3. Dropdown outside click handler
    document.addEventListener('click', (e) => {
        if (!e.target.closest('.lang-dropdown')) {
            const menu = document.getElementById('langMenu');
            if (menu) menu.classList.remove('show');
        }
    });
});

function toggleLangMenu(event) {
    event.stopPropagation();
    const menu = document.getElementById('langMenu');
    if (menu) menu.classList.toggle('show');
}

function setLanguage(lang) {
    if (!i18n[lang]) return;
    currentLang = lang;
    localStorage.setItem('intgmdbot_lang', lang);
    document.body.setAttribute('data-lang', lang);

    // Update Dropdown Label
    const labels = { ru: '🇷🇺 RU', be: '🇧🇾 BE', en: '🇬🇧 EN' };
    const labelSpan = document.getElementById('currentLangLabel');
    if (labelSpan) labelSpan.textContent = labels[lang];

    // Apply translations
    const elements = document.querySelectorAll('[data-i18n]');
    elements.forEach(el => {
        const key = el.getAttribute('data-i18n');
        if (i18n[lang][key]) {
            el.innerHTML = i18n[lang][key];
        }
    });

    const menu = document.getElementById('langMenu');
    if (menu) menu.classList.remove('show');
}

function toggleTheme() {
    const currentTheme = document.body.getAttribute('data-theme') || 'dark';
    const newTheme = currentTheme === 'dark' ? 'light' : 'dark';
    setTheme(newTheme);
}

function setTheme(theme) {
    document.body.setAttribute('data-theme', theme);
    localStorage.setItem('intgmdbot_theme', theme);

    const themeIcon = document.getElementById('themeIcon');
    if (themeIcon) {
        themeIcon.className = theme === 'dark' ? 'fas fa-moon' : 'fas fa-sun';
    }
}

// Interactive Demo Tab Switcher
function switchDemoTab(tabName) {
    const tabBtns = document.querySelectorAll('.stage-tab-btn');
    const tabPanels = document.querySelectorAll('.demo-tab-panel');

    tabBtns.forEach(btn => btn.classList.remove('active'));
    tabPanels.forEach(panel => panel.classList.remove('active'));

    const activeBtn = document.querySelector(`.stage-tab-btn[onclick*="${tabName}"]`);
    const activePanel = document.getElementById(`panel-${tabName}`);

    if (activeBtn) activeBtn.classList.add('active');
    if (activePanel) activePanel.classList.add('active');
}
