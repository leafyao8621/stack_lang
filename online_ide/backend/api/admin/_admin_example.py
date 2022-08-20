from django.contrib import admin
from ..models import Example

class ExampleAdmin(admin.ModelAdmin):
    list_display = (
        "file_name",
    )

admin.site.register(Example, ExampleAdmin)
