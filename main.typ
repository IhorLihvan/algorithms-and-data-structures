#import "@preview/marginalia:0.3.1" as marginalia

#set page(
  paper: "a4",
  header: context {
    if here().page() == 1 { return }
    align(center, text(size: 9pt, fill: luma(120))[Технічна книга курсової роботи — Алгоритми і Структури Даних])
  },
  footer: context {
    if here().page() == 1 { return }
    align(center, text(size: 10pt)[#here().page()])
  }
)

// Поля сторінки та колонка приміток праворуч (стиль kaobook)
// Праве поле = far + width + sep = 6.5cm
#show: marginalia.setup.with(
  inner: (far: 2.5cm, width: 0cm, sep: 0cm),
  outer: (far: 1.3cm, width: 4.5cm, sep: 0.7cm),
  top: 2.5cm,
  bottom: 2.5cm,
)


// Налаштування шрифтів та тексту
#set text(font: "Liberation Serif", size: 11pt, lang: "uk")
#set par(justify: true, leading: 0.65em)
#set heading(numbering: "1.1.")

// --- ТИТУЛЬНА СТОРІНКА ---
#page(margin: 2.5cm)[
  #v(4cm)
  #align(center)[
    #text(size: 20pt, weight: "bold")[КУРСОВА РОБОТА] \
    #v(1cm)
    #text(size: 14pt)[Тема: Дослідження та реалізація алгоритмів обробки даних] \
    #v(2cm)
    #text(size: 12pt)[Виконав: студент групи КН-...\
    Ігор Ліхван] \
    #v(1cm)
    #text(size: 12pt)[Керівник: Літинський Святослав] \
    #v(3cm)
    #text(size: 10pt)[Львів --- 2026]
  ]
]

#pagebreak()

// --- ЗМІСТ ---
#outline(title: "Зміст", indent: 1.5em)
#pagebreak()

// --- ПІДКЛЮЧЕННЯ РОЗДІЛІВ ---
#include "chapter1.typ"
#include "chapter4.typ"