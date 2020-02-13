<?php

class HeimdalHelpers {

    private $config;

    public function __construct($config) {
        $this->config = $config;
    }

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
        $accessplatform = isset($config['heimdal_client_url']) ? $config['heimdal_client_url'] : FALSE;

        $heimdal_url = '';
        if (!empty($redirect_url)) {
            $redirect = '&redirect_uri=' . $redirect_url;
        }
        if ($accessplatform) {
            $heimdal_url .= $config['heimdal_client_url'] . 'oauth/authorize?response_type=code&client_id=' . $config['heimdal_client_id'] . $redirect;
        }

        return $heimdal_url;
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
        $accessplatform = isset($config['heimdal_client_url']) ? $config['heimdal_client_url'] : FALSE;

        $heimdal_url = '';
        if (!empty($redirect_url)) {
            $redirect = '&redirect_uri=' . $redirect_url;
        }
        if ($accessplatform) {
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
