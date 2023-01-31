valid syntax for Json file (to parse):

	[
		{
			"name": "some_string (value)",
			"value": "some_string (value)",
			"type": "some_string (value)",
			"readonly": "some_string (value)"
		},
		{
			"name": "some_string (value)",
			"value": "some_string (value)",
			"type": "some_string (value)",
			"readonly": "some_string (value)"
		},
		...
	]

some notes about valid values for the written keys:

	1) name key     -> actually the value is label name that will be written next to the main object
				       (for example main object can be a lineEdit or pushButton or etc)

	2) value key    -> default value for the main object

	3) type key     -> this is type of the main object (for example lineEdit, pushButton, ...)

							NOTE: valid types:
									- bool (checkbox representative)

											NOTE: for bool type, default value can be "checked-true" or "checked-false"
												  and if you don't determine the default value, by default it will be set
												  "checked-true" value .

									- string (lineEdit representative)
									- string list (combobox representative)
									- number_i (simple spin box for integer numbers)
									- number_ui (simple spin box for non-negative integer numbers)
									- number_f (double spin box for double numbers)
									- number_uf (double spin box for non-negative double numbers)
									- file (lineEdit and fileBrowser representative)


	4) readonly key -> the value for this key is true or false (if it's false, you can't change value of this object)
