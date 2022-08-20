from django.contrib import admin
from ..models import File

class FileAdmin(admin.ModelAdmin):
    list_display = (
        "created_by",
        "file_name"
    )

admin.site.register(File, FileAdmin)
