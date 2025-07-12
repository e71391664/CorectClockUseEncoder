# Git Commands Guide для Arduino проекту

## 1. ІНІЦІАЛІЗАЦІЯ РЕПОЗИТОРІЮ

### Налаштування Git конфігурації
```bash
# Встановлення email користувача (обов'язково для комітів)
git config user.email "your-email@gmail.com"

# Встановлення імені користувача (обов'язково для комітів)
git config user.name "your-username"
```

### Створення нового репозиторію
```bash
# Перехід в папку проекту
cd CorectClockUseEncoder

# Ініціалізація Git репозиторію
git init

# Додавання всіх файлів до staging area
git add .

# Перший коміт з повідомленням
git commit -m "Initial commit: CorectClockUseEncoder project"
```

### Налаштування віддаленого репозиторію
```bash
# Додавання GitHub репозиторію як origin
git remote add origin https://github.com/username/repo-name.git

# Перейменування гілки master на main (сучасний стандарт)
git branch -M main

# Перший пуш з встановленням upstream
git push -u origin main
```

---

## 2. ОНОВЛЕННЯ ПРОЕКТУ

### Додавання змін
```bash
# Додавання конкретного файлу
git add filename.ino

# Додавання всіх змінених файлів
git add .

# Додавання всіх файлів (включаючи видалені)
git add -A
```

### Коміт змін
```bash
# Коміт з повідомленням
git commit -m "Add new feature: encoder support"

# Коміт всіх змінених файлів (без git add)
git commit -am "Update display logic"
```

### Пуш змін
```bash
# Звичайний пуш (якщо гілка вже налаштована)
git push

# Пуш з встановленням upstream
git push -u origin main

# Force push (перезаписує історію на сервері)
git push -u origin main --force
```

---

## 3. РОБОТА З ГІЛКАМИ

### Створення нової гілки
```bash
# Створення та перехід на нову гілку
git checkout -b feature/new-encoder

# Або створення гілки без переходу
git branch feature/new-encoder
```

### Переключення між гілками
```bash
# Перехід на існуючу гілку
git checkout main

# Перехід на останню гілку (Git 2.23+)
git switch main
```

### Мердж гілок
```bash
# Перехід на цільову гілку
git checkout main

# Мердж feature гілки в main
git merge feature/new-encoder

# Пуш змерджених змін
git push origin main
```

---

## 4. КОРИСНІ КОМАНДИ ДЛЯ ПЕРЕГЛЯДУ

### Статус та історія
```bash
# Перегляд статусу репозиторію
git status

# Перегляд історії комітів
git log --oneline

# Перегляд віддалених репозиторіїв
git remote -v
```

### Відміна змін
```bash
# Відміна змін в конкретному файлі
git checkout -- filename.ino

# Відміна останнього коміту (зберігає зміни)
git reset --soft HEAD~1

# Повна відміна останнього коміту
git reset --hard HEAD~1
```

---

## 5. ВИРІШЕННЯ ПРОБЛЕМ

### Якщо пуш не працює
```bash
# Перевірка віддаленого репозиторію
git remote -v

# Force push (увага: перезаписує історію!)
git push -u origin main --force
```

### Якщо відкрився редактор
```bash
# У vim: натисніть Esc, потім :q! та Enter
# У nano: натисніть Ctrl+X, потім N та Enter
```

### Якщо конфлікт мерджу
```bash
# Відміна мерджу
git merge --abort

# Або вирішення конфліктів вручну
# Потім: git add . та git commit
```

---

## 6. ТИПОВІ СЦЕНАРІЇ

### Створення нового проекту
```bash
mkdir NewProject
cd NewProject
git init
git config user.email "your-email@gmail.com"
git config user.name "your-username"
git add .
git commit -m "Initial commit"
git remote add origin https://github.com/username/repo.git
git branch -M main
git push -u origin main
```

### Оновлення існуючого проекту
```bash
git add .
git commit -m "Update: add new feature"
git push
```

### Створення feature гілки
```bash
git checkout -b feature/new-feature
# ... робота над функцією ...
git add .
git commit -m "Add new feature"
git push -u origin feature/new-feature
# Потім мердж через GitHub або локально
``` 