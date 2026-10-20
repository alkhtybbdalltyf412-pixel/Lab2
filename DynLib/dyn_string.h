#pragma once
#include <cstdio>

// قراءة سطر من stream حرف حرف.
// كل حرف جديد: malloc لبلوك أكبر بحرف، نسخ القديم، free للقديم.
// ترجع سلسلة C جديدة (لازم المستدعي يعمل free)، أو nullptr عند فشل الحجز.
char* read_line_malloc(std::FILE* in);

// قراءة سطر بدفعات (chunk <= 5): أول دفعة calloc، بعدها realloc.
// ترجع سلسلة C جديدة (لازم المستدعي يعمل free)، أو nullptr عند فشل الحجز.
char* read_line_calloc(std::FILE* in, int chunk);

// معالجة المهمة 5: حذف الأرقام الفردية وإضافة "++" بعد كل رقم زوجي
// (إلا إذا كان آخر محرف بالسلسلة الأصلية).
// ترجع سلسلة جديدة محجوزة بـ realloc (الأصلية بتتحرر داخلياً).
// المستدعي يعمل free للنتيجة. ترجع nullptr عند فشل الحجز.
char* process_digits_dyn(char* s);