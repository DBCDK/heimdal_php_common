<?php

class HeimdalHelpers {

    /*
    * Configuration data for Heimdal client
    * client url, id and secret
    *
    * @var array
    */
    private $config;

    public function __construct($config) {
        if ($config['heimdal_client_url'] === NULL || $config['heimdal_client_url'] === '' ||
            $config['heimdal_client_id'] === NULL || $config['heimdal_client_id'] === '' ||
            $config['heimdal_client_secret'] === NULL || $config['heimdal_client_secret'] === '') {
            throw new Exception('Heimdal has not been configured correctly');
        }
        $this->config = $config;
    }

    /**
    * Get the configuration for the Heimdal Client
    * @return array
    */
    public function getConfig() {
        return $this->config;
    }

    /**
    * Get the url for heimdal login
    * @return string
    */
    public function heimdal_get_login_url($relative_path = '') {
        $config = $this->getConfig();
        $redirect_url = HeimdalHelpers::heimdal_get_full_url($relative_path);
        $access_platform = isset($config['heimdal_client_url']) ? $config['heimdal_client_url'] : FALSE;

        $heimdal_url = '';
        if (!empty($redirect_url)) {
            $redirect = '&redirect_uri=' . $redirect_url;
        }
        if ($access_platform) {
            $heimdal_url .= $config['heimdal_client_url'] . 'oauth/authorize?response_type=code&client_id=' . $config['heimdal_client_id'] . $redirect;
        }

        return $heimdal_url;
    }

    /**
    * Do a request for attributes and parse response into an array
    * @param string $auth_code
    *   Authorization code from Heimdal
    * @param array $config
    *   Service configuration
    * @return array | bool
    *   Json response parsed into an array or empty array if
    *   something goes wrong
    */
    public function getAttributes($auth_code) {
        //exchange authorization code for access token
        $config = $this->getConfig();
        $curl = new HeimdalCurl();
        $token = $curl->getAccessToken($auth_code, $config);
        if(empty($token)) {
            return FALSE;
        }

        $token_as_json = json_decode($token);
        $bearer = $token_as_json->access_token;
        // we are logged in - set access_token (bearer) in SESSION for later use
        $_SESSION['heimdal']['access_token'] = $bearer;
        $_SESSION['heimdal']['logged_in'] = 1;

        $attributes = $curl->getUserInfo($bearer, $config);
        if ($curl->checkMe()) {
            return (!empty($attributes)) ? json_decode($attributes, true) : array();
        }
        return false;
    }

    /**
    * Get the url for heimdal login
    *
    * @param string $access_token
    * @param string $relative_path
    *
    * @return string
    */
    public function heimdal_get_logout_url($access_token, $relative_path = '') {
        $config = $this->getConfig();
        $redirect_url = HeimdalHelpers::heimdal_get_full_url($relative_path);
        $access_platform = isset($config['heimdal_client_url']) ? $config['heimdal_client_url'] : FALSE;

        $heimdal_url = '';
        if (!empty($redirect_url)) {
            $redirect = '&redirect_uri=' . $redirect_url;
        }
        if ($access_platform) {
            $heimdal_url .= $config['heimdal_client_url'] . 'logout/?access_token=' . $access_token . $redirect;
        }

        return $heimdal_url;
    }

    /**
    * Returns absolute url - url is made up of _SERVER variables, drupal base_path and
    * given relative path
    *
    * @param $relative_path
    *  do NOT add '/' to beginning of parameter
    *
    * @return string
    */
    public static function heimdal_get_full_url($relative_path = '') {
        if (isset($_SERVER['HTTPS']) &&
        ($_SERVER['HTTPS'] == 'on' || $_SERVER['HTTPS'] == 1) ||
        isset($_SERVER['HTTP_X_FORWARDED_PROTO']) &&
        $_SERVER['HTTP_X_FORWARDED_PROTO'] == 'https') {
            $protocol = 'https://';
        }
        else {
            $protocol = 'http://';
        }
        $host = $_SERVER['HTTP_HOST'];
        $path = base_path();
        return $protocol . $host . $path . $relative_path;
    }
}
