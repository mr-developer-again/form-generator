# Form Generator

### Json Valid Syntax

	[
		{
			"name": "some_string",
			"type": "some_string",
			"default value": "some_string",
			"string list values": ["value1", "value2", ...],
			"description": "some_string",
			"readonly": "some_string"
		},
		{
			...
		},
		...
	]

***Some Notes***
> 1. ***Case sensitivity is ignored for all keys***

> 2. ***Order is not important in writting the keys***


### Valid Values For The Json File Keys ("name", "type", "default value", ...)

	1. "name" key
		actually the value is label name that will be written next to the main object
		(for example main object can be a lineEdit or pushButton or etc)

	2. "type" key
		this is type of the main object (for example lineEdit, pushButton, ...);
		Valid values for this key are listed below

			valid types:

				* "bool" (checkbox representative)

					NOTE:
						for bool type, default value can be "checked-true" or
						"checked-false"	and if you don't determine the
						default value, by default it will be set
						"checked-true" value .
				
				* "string" (lineEdit representative)
				* "string list" (combobox representative)
				* "number_i" (simple spin box for integer numbers)
				* "number_ui" (simple spin box for non-negative integer numbers)
				* "number_f" (double spin box for double numbers)
				* "number_uf" (double spin box for non-negative double numbers)
				* "file" (lineEdit and fileBrowser representative)

	3. "string list values" key
		value for this key is a array of values that you want to set for the combo-box


	4. "default value" key (optional)
		default value for the main object

			NOTE: you can't set the "default value" key for "string" and "file" types.

	5. "description" key (optional)
		if you want to write some description inside lineEdit, you can use this key

			NOTE: this key (description) is used only for the types which have lineEdit (file, string)

	6. "readonly" key (optional)
		the value for this key is true or false (if it's false, you can't change value of this object)
		(deafult value for this key (if you don't determine it), is false)

## Example
the following form has been created from the Json below

```
[
	{
		"name": "first name",
		"description": "first name",
		"type": "string",
		"readonly": "false"
	},
	{
		"name": "last name",
		"description": "last name",
		"type": "string",
		"readonly": "true"
	},
	{
		"name": "age",
		"default value": "19",
		"type": "number_ui",
		"readonly": "false"
	},
	{
		"name": "height",
		"default value": "-1.0",
		"type": "number_f",
		"readonly": "false"
	},
	{
		"name": "some integer",
		"default value": "-5",
		"type": "number_i",
		"readonly": "true"
	},
	{
		"name": "some double",
		"default value": "3.14",
		"type": "number_uf",
		"readonly": "false"
	},
	{
		"name": "is alive",
		"default value": "checked-true",
		"type": "bool",
		"readonly": "true"
	},
	{
		"name": "CV Path",
		"description": "Enter your CV path here",
		"type": "file",
		"readonly": "false"
	},
	{
		"name": "level",
		"type": "string list",
		"string list values": ["familiar", "beginner", "junior", "mid-level", "skilled", "expert"],
		"default value": "beginner"
	}
]

```

and, the output:

![image](https://user-images.githubusercontent.com/112639283/216704085-9438084b-d5ce-4176-aaf8-0b26b72a7525.png)
_<center>Generated Form<center>_
