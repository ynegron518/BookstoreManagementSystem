let cart = [];

// Add to Cart
function addToCart(title, price, quantity = 1) {
    const itemIndex = cart.findIndex(item => item.title === title);
    if (itemIndex !== -1) {
        cart[itemIndex].quantity += quantity;
    } else {
        cart.push({ title, price, quantity });
    }
    updateCartDisplay();
}

// Update Cart Display
function updateCartDisplay() {
    const cartItemsDiv = document.getElementById("cartItems");
    const cartTotalSpan = document.getElementById("cartTotal");
    cartItemsDiv.innerHTML = "";

    let total = 0;
    cart.forEach((item, index) => {
        const itemTotal = item.price * item.quantity;
        total += itemTotal;

        const itemDiv = document.createElement("div");
        itemDiv.classList.add("cart-item");
        itemDiv.innerHTML = `
            <p><strong>${item.title}</strong></p>
            <p>Price: $${item.price.toFixed(2)}</p>
            <p>Quantity: 
                <button onclick="updateQuantity(${index}, -1)">-</button>
                ${item.quantity}
                <button onclick="updateQuantity(${index}, 1)">+</button>
            </p>
            <p>Total: $${itemTotal.toFixed(2)}</p>
            <button onclick="removeFromCart(${index})">Remove</button>
        `;
        cartItemsDiv.appendChild(itemDiv);
    });

    cartTotalSpan.innerText = total.toFixed(2);
}

// Update Item Quantity in Cart
function updateQuantity(index, change) {
    cart[index].quantity += change;
    if (cart[index].quantity <= 0) {
        cart.splice(index, 1);
    }
    updateCartDisplay();
}

// Remove Item from Cart
function removeFromCart(index) {
    cart.splice(index, 1);
    updateCartDisplay();
}

// Checkout
function checkout() {
    if (cart.length === 0) {
        document.getElementById("checkoutStatus").innerText = "Cart is empty!";
        document.getElementById("checkoutStatus").className = "error";
        return;
    }
    
    cart = [];
    updateCartDisplay();
    document.getElementById("checkoutStatus").innerText = "Thank you for your purchase!";
    document.getElementById("checkoutStatus").className = "success";
}

// Load inventory
async function loadInventory() {
    try {
        const response = await fetch('/api/inventory');
        const text = await response.text();
        document.getElementById("inventoryList").innerHTML = text || '<p class="error">No inventory available.</p>';
    } catch (error) {
        console.error('Error loading inventory:', error);
    }
}

// Search books
async function searchBooks() {
    const searchTerm = document.getElementById("searchInput").value.trim();
    try {
        const response = await fetch(`/api/search?term=${encodeURIComponent(searchTerm)}`);
        const text = await response.text();
        document.getElementById("searchResults").innerHTML = text || '<p class="error">No results found</p>';
    } catch (error) {
        console.error('Search error:', error);
    }
}

// Place an order
async function placeOrder(event) {
    event.preventDefault();
    const title = document.getElementById("orderTitle").value;
    const quantity = parseInt(document.getElementById("orderQuantity").value);
    
    addToCart(title, 0, quantity); // Use 0 as price placeholder
    document.getElementById("orderStatus").innerText = "Book added to cart.";
}

// Add a new book to wishlist
async function addToWishlist(event) {
    event.preventDefault();
    const title = document.getElementById("wishlistTitle").value;
    document.getElementById("wishlistList").innerHTML += `<p>${title} added to wishlist.</p>`;
}

// Load Order History
async function loadOrderHistory() {
    try {
        const response = await fetch('/api/order_history');
        const text = await response.text();
        document.getElementById("orderHistoryList").innerHTML = text;
    } catch (error) {
        console.error('Order history load error:', error);
    }
}

// Submit Review
function submitReview(event) {
    event.preventDefault();
    const title = document.getElementById("reviewTitle").value;
    const content = document.getElementById("reviewContent").value;
    document.getElementById("reviewsList").innerHTML += `<p><strong>${title}</strong>: ${content}</p>`;
    document.getElementById("reviewStatus").innerText = "Review submitted.";
}
