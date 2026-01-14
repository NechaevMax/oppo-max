#!/usr/bin/env python3
"""Простой генератор документации по исходному коду (без сторонних зависимостей).

Задача:
- пройти по заголовочным файлам `include/menu/*.h`;
- извлечь названия классов и их публичные методы/конструкторы;
- сформировать Markdown-файл.

Это не Doxygen, но подходит как пример «инструмента построения документации по исходному коду»
в рамках ПР 2.1.5.
"""

from __future__ import annotations

import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
INCLUDE_DIR = ROOT / "include" / "menu"
OUT = ROOT / "docs" / "API_DOCUMENTATION.md"

CLASS_RE = re.compile(r"^\s*class\s+(\w+)")
PUBLIC_RE = re.compile(r"^\s*public:\s*$")
PRIVATE_PROTECTED_RE = re.compile(r"^\s*(private|protected):\s*$")

# Пример строки, которую хотим поймать:
#   std::vector<MenuItem> loadFromFile(const std::string& filename) const override;
METHOD_LINE_RE = re.compile(r";\s*$")


def _looks_like_declaration(line: str, current_class: str) -> bool:
    s = line.strip()
    if not s or not METHOD_LINE_RE.search(s):
        return False
    if "(" not in s or ")" not in s:
        return False
    if s.startswith("throw "):
        return False

    # Отбрасываем выражения внутри inline-реализаций (присваивания и т.п.).
    if "=" in s and "default" not in s and not re.search(r"=s*0", s):
        return False

    prefix = s.split("(", 1)[0].strip()
    # Если перед '(' нет пробела, это либо вызов функции, либо ctor/dtor.
    if " " not in prefix:
        if prefix == current_class or prefix == f"~{current_class}":
            return True  # ctor/dtor
        return False

    return True


def parse_header(path: Path) -> dict[str, list[str]]:
    """Возвращает: {ClassName: [method signatures...]}"""
    result: dict[str, list[str]] = {}
    current_class: str | None = None
    in_public = False

    for line in path.read_text(encoding="utf-8", errors="ignore").splitlines():
        m = CLASS_RE.match(line)
        if m:
            current_class = m.group(1)
            result.setdefault(current_class, [])
            in_public = False
            continue

        if current_class is None:
            continue

        if PUBLIC_RE.match(line):
            in_public = True
            continue
        if PRIVATE_PROTECTED_RE.match(line):
            in_public = False
            continue

        if in_public and _looks_like_declaration(line, current_class):
            result[current_class].append(line.strip())

    return result


def main() -> None:
    headers = sorted(INCLUDE_DIR.glob("*.h"))
    if not headers:
        raise SystemExit(f"No headers found in {INCLUDE_DIR}")

    all_classes: dict[str, list[str]] = {}
    for h in headers:
        parsed = parse_header(h)
        for cls, methods in parsed.items():
            all_classes.setdefault(cls, [])
            all_classes[cls].extend(methods)

    lines: list[str] = []
    lines.append("# API документация (автогенерация)\n")
    lines.append("Источник: заголовочные файлы `include/menu/*.h`.\n")

    for cls in sorted(all_classes.keys()):
        lines.append(f"## {cls}\n")
        methods = all_classes[cls]
        if not methods:
            lines.append("_Публичные объявления методов не обнаружены._\n")
            continue
        lines.append("Публичные методы/конструкторы:\n")
        for meth in methods:
            lines.append(f"- `{meth}`")
        lines.append("")

    OUT.parent.mkdir(parents=True, exist_ok=True)
    OUT.write_text("\n".join(lines), encoding="utf-8")
    print(f"Generated: {OUT}")


if __name__ == "__main__":
    main()
