#include <iostream>
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/cef_request_handler.h"

// Single Script Browser Using CEF

class SimpleHandler : public CefClient, public CefLifeSpanHandler, public CefRequestHandler {
public:
    SimpleHandler() {}
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
    CefRefPtr<CefRequestHandler> GetRequestHandler() override { return this; }

    bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect) override {
        std::string url = request->GetURL();
        std::cout << "Navigating to: " << url << std::endl;

        // Simple URL block (example)
        if (url.find("malicious.com") != std::string::npos) {
            std::cout << "Blocked malicious URL: " << url << std::endl;
            return true; // Block the request
        }
        return false;
    }

    IMPLEMENT_REFCOUNTING(SimpleHandler);
};

int main(int argc, char* argv[]) {
    // Initialize CEF
    CefMainArgs main_args(argc, argv);
    CefSettings settings;
    settings.no_sandbox = true; // Simplicity: Disable sandboxing

    CefRefPtr<CefApp> app;
    if (!CefInitialize(main_args, settings, app, nullptr)) {
        std::cerr << "CEF Initialization failed!" << std::endl;
        return -1;
    }

    // Create the browser window
    CefBrowserSettings browser_settings;
    CefWindowInfo window_info;
    window_info.SetAsPopup(nullptr, "Single-Script Browser");
    CefRefPtr<SimpleHandler> handler = new SimpleHandler();

    CefBrowserHost::CreateBrowser(window_info, handler, "http://example.com", browser_settings, nullptr, nullptr);

    // Run the CEF message loop
    CefRunMessageLoop();

    // Shutdown CEF
    CefShutdown();
    return 0;
}
