# heimdal_php_common

A Drupal module[*](#note) for integrating with the Hejmdal service (Adgangsplatformen).

This client supports
--------------------
* Authorization Request
* Access Token Request
* User Info Request
* Log Out Request

from the [Hejmdal oAuth2 login flow](https://github.com/DBCDK/hejmdal/blob/master/docs/oauth2.md)

Requirements
------------
[microcurl](https://github.com/DBCDK/microcurl)

In order to use this module in your client, you need to acquire access to Hejmdal/Adgangsplatformen.

Contact:
[DBC Kundeservice](mailto:kundeservice.dbc.dk)

Your client must then be configured by DBC, and this configuration is required by this module,
in order to use it.

*Footnotes:*

<a name="note"> * </a>The intention is that this module is changed to a PHP Library,
but this can only be done when bibliotek.dk is able to use composer.
